/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/debug.hh>
#include "ros-overload.hh"

using namespace ros;
using ROSBinding::ROSTopic;
using ROSBinding::TopicMsg;
using ROSBinding::UValuePtr;
using ROSBinding::UValueConstPtr;

GD_CATEGORY(Ros.Overload);


Subscriber
NodeHandleExt::subscribe(ROSTopic* rt)
{
  SubscribeOptionsExt ops;
  ops.initWithRosTopic(rt);
  ops.transport_hints = TransportHints();
  return this->NodeHandle::subscribe(ops);
}

Publisher
NodeHandleExt::advertise(ROSTopic* rt)
{
  AdvertiseOptionsExt ops;
  ops.initWithRosTopic(rt);
  return this->NodeHandle::advertise(ops);
}


void
SubscribeOptionsExt::initWithRosTopic(ROSTopic* rt)
{
  if (!rt)
    return;

  const TopicMsg* tm = rt->topic_msg_get();

  topic = rt->topic_get();
  queue_size = rt->queue_size_get();
  md5sum = tm->md5sum_get();
  datatype = tm->type_get();

  GD_FINFO_TRACE("Initializing ROS Topic with %s %d %s %s",
                 topic, queue_size, datatype, md5sum);

  helper = SubscriptionCallbackHelperPtr(
             new SubscriptionCallbackHelperExt(
               boost::bind<void>(&ROSTopic::msg_callback, rt, _1),
               boost::bind<urbi::UValue*>(&TopicMsg::deserialize, tm, _1, _2),
               tm->is_fixed_size()));
}


void
AdvertiseOptionsExt::initWithRosTopic(ROSTopic* rt)
{
  if (!rt)
    return;

  const TopicMsg* tm = rt->topic_msg_get();

  topic = rt->topic_get();
  queue_size = rt->queue_size_get();
  md5sum = tm->md5sum_get();
  datatype = tm->type_get();
  has_header = tm->has_header();
  /* This slot seems not to be important to ROS,
     so we don't bother to define it for now. */
  message_definition = "Dummy";

  // Define callbacks on Connect and Disconnect.
  connect_cb = boost::bind<void>(&ROSTopic::subscriber_connect, rt, _1);
  disconnect_cb = boost::bind<void>(&ROSTopic::subscriber_disconnect, rt, _1);
}


SubscriptionCallbackHelperExt::SubscriptionCallbackHelperExt(
    Callback callback, SerializerCB deserializer, bool fixed_size)
  : callback_(callback)
  , deserializer_(deserializer)
  , fixed_size_(fixed_size)
{}


VoidConstPtr
SubscriptionCallbackHelperExt::deserialize(
    const SubscriptionCallbackHelperDeserializeParams& params)
{
  return VoidConstPtr(deserializer_(params.buffer, params.length));
}


void
SubscriptionCallbackHelperExt::call(
  SubscriptionCallbackHelperCallParams& params)
{
  // Transform the VoidConstPtr to a UValuePtr without freeing the data.
  callback_(
    boost::const_pointer_cast<urbi::UValue>(
      boost::static_pointer_cast<urbi::UValue const>(
        params.event.getMessage())));
}


const std::type_info&
SubscriptionCallbackHelperExt::getTypeInfo()
{
  // "Random" typeid because of the parent abstract class.
  return typeid(ROSBinding::TopicMsg);
}

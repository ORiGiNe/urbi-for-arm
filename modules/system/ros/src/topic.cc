/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/exception.hh>
#include <libport/foreach.hh>
#include <libport/thread.hh>
#include <ros/ros.h>
#include <ros/topic_manager.h>

#include "topic.hh"

using ROSBinding::ROSTopic;
using libport::Exception;

// Declare our UObject to Urbi.
UStart(ROSTopic);

# define THROW(...)                             \
  throw Exception(libport::format(__VA_ARGS__))

GD_CATEGORY(Ros.Topic);

ROSTopic::ROSTopic(const std::string& name)
  : urbi::UObject(name)
  , queue_size_(ROS_QUEUE_SIZE)
  , inst_type_(TYPE_NOT_DEFINED)
{
  UBindFunction(ROSTopic, init);
  UBindFunctionRename(ROSTopic, topic_get, "name");
  UBindThreadedFunction(ROSTopic, subscribe, urbi::LOCK_INSTANCE);
  UBindThreadedFunction(ROSTopic, advertise, urbi::LOCK_INSTANCE);
  UBindThreadedFunction(ROSTopic, unsubscribe, urbi::LOCK_INSTANCE);
  UBindThreadedFunction(ROSTopic, unadvertise, urbi::LOCK_INSTANCE);
  UBindFunction(ROSTopic, publish);
  UBindFunctionRename(ROSTopic, publish, "<<");
  UBindFunctionRename(ROSTopic, subscriber_number_get, "subscriberCount");

  UBindVar(ROSTopic, structure);

  UBindEventRename(ROSTopic, ev_msg, "onMessage");
  UBindEventRename(ROSTopic, sub_connect, "onConnect");
  UBindEventRename(ROSTopic, sub_disconnect, "onDisconnect");
}


int
ROSTopic::init(const std::string& topic)
{
  if (inst_type_)
    THROW("Instance already running on %s", topic_);

  if (topic.empty())
    THROW("Topic name cannot be empty.");

  topic_ = ROSTools::clean(topic);
  if (topic_.empty())
    THROW("Invalid character in %s", topic);

  return 0;
}


ROSTopic::~ROSTopic()
{
  switch (inst_type_)
  {
    case TYPE_SUBSCRIBER:
      unsubscribe();
      break;

    case TYPE_EMITTER:
      unadvertise();
      break;

    default:
      ;
  }
}


void
ROSTopic::subscribe()
{
  if (inst_type_)
    THROW("cannot subscribe: already defined on %s (%s)", topic_,
          (inst_type_ == TYPE_EMITTER) ? "emitter" : "subscriber");

  if (!ROSTools::instance_get()->master_check())
    THROW("Unable to contact ROS master node.");

  ros::master::V_TopicInfo topic_info;
  if (!ros::master::getTopics(topic_info))
    THROW("Unable to retrieve topic list.");

  std::string found_type;
  foreach(const ros::master::TopicInfo& item, topic_info)
    if (topic_ == item.name)
    {
      found_type = item.datatype;
      break;
    }

  if (found_type.empty())
    THROW("The given topic is not registered.");

  // This throws libport::Exception when the type does not exist (rare).
  topic_msg_ = TopicMsg::fetch(found_type);
  inst_type_ = TYPE_SUBSCRIBER;
  structure = topic_msg_->structure_get();
  // ROS Subscription through our overload.
  ros_sub_ = ROSTools::NodeHandle()->subscribe(this);
}


void
ROSTopic::unsubscribe()
{
  if (inst_type_ == TYPE_SUBSCRIBER)
  {
    libport::BlockLock l(msg_lock_);
    // ros_sub_.unsubscribe() is private, but called by the destructor.
    ros_sub_ = ros::Subscriber();
    inst_type_ = TYPE_NOT_DEFINED;
  }
}


void
ROSTopic::advertise(std::string msg_type)
{
  GD_FINFO_TRACE("Advertising %s", msg_type);
  if (inst_type_)
    THROW("cannot advertise: already defined on %s (%s)", topic_,
          (inst_type_ == TYPE_EMITTER) ? "emitter" : "subscriber");

  if (!ROSTools::instance_get()->master_check())
    throw std::runtime_error("Unable to contact ROS master node.");

  // Get the topics (and their types) via ROS API.
  ros::master::V_TopicInfo topic_info;
  if (!ros::master::getTopics(topic_info))
    throw std::runtime_error("Unable to retrieve topic list.");

  std::string found_type;
  // Looking whether the topic is already registered, and if so its type.
  foreach (const ros::master::TopicInfo& item, topic_info)
    if (topic_ == item.name)
    {
      found_type = item.datatype;
      break;
    }

  if (!found_type.empty() && msg_type != found_type)
    THROW("%s is already registered with type %s", topic_, found_type);
  else if (msg_type.empty())
    msg_type = found_type;

  /// This throws libport::Exception when the type does not exist.
  topic_msg_ = TopicMsg::fetch(msg_type);
  inst_type_ = TYPE_EMITTER;
  structure = topic_msg_->structure_get();
  // Register the node as a publisher to ROS.
  ros_pub_ = ROSTools::NodeHandle()->advertise(this);
}


void
ROSTopic::unadvertise()
{
  if (inst_type_ == TYPE_EMITTER)
  {
    libport::BlockLock l(msg_lock_);
    // unadvertise() is private, but called with ros_pub_ destructor.
    ros_pub_ = ros::Publisher();
    inst_type_ = TYPE_NOT_DEFINED;
  }
}


int
ROSTopic::subscriber_number_get() const
{
  if (!ROSTools::instance_get()->master_check())
    THROW("Unable to communicate with master.");
  return ros::TopicManager::instance()->getNumSubscribers(topic_);
}


void
ROSTopic::publish(urbi::UDictionary pl)
{
  if (inst_type_ != TYPE_EMITTER)
    THROW("Invalid call to publish. Call advertise first.");

  if (!topic_msg_)
    THROW("No TopicMsg instance.");

  // Calls the ROS Internals.
  ros::SerializedMessage message;
  urbi::UDictionary* d = new urbi::UDictionary;
  pl.swap(*d);
  boost::function<ros::SerializedMessage(void)> ser_function =
    boost::bind<ros::SerializedMessage>(&TopicMsg::serialize, &*topic_msg_, d);

  ros::TopicManager::instance()->publish(topic_, ser_function, message);
  delete d;
}

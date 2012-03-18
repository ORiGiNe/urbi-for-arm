/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ROS_OVERLOAD_HH
# define ROS_OVERLOAD_HH

# include <ros/node_handle.h>

namespace ros
{
  class NodeHandleExt;
}

# include "topic.hh"


/**
 * @brief All the classes that belongs to ROS universe.
 * Used here because we overload some ROS Internal classes.
 */
namespace ros
{
  /**
   * @brief Provide us customs subscribe and advertise.
   * Inherit from ros::NodeHandle. subscribe and advertise are untemplated,
   * as opposed to original class.
   */
  class NodeHandleExt
    : public NodeHandle
  {
    public:
      /**
       * @brief Simple contructor, takes an optional namespace in \a ns.
       * Just calls the parent constructor with \a ns.
       */
      NodeHandleExt(const std::string& ns = std::string());

      /**
       * @brief Subscribe a topic without template.
       * Takes its datas from \a rt.
       * @return an empty Subscriber if rt is NULL.
       */
      Subscriber subscribe(ROSBinding::ROSTopic* rt);

      /**
       * @brief Advertise a topic without template.
       * Takes its datas from \a rt.
       * @return an empty Publisher if rt is NULL.
       */
      Publisher advertise(ROSBinding::ROSTopic* rt);
  };


  /// Extension of SubscribeOptions (untemplated).
  class SubscribeOptionsExt
    : public SubscribeOptions
  {
    public:
      /// Just calls the parent constructor.
      SubscribeOptionsExt();

      /**
       * @brief Initialize SubscribeOptions with \a rt.
       * Does nothing if rt is NULL.
       */
      void initWithRosTopic(ROSBinding::ROSTopic* rt);
  };

  /// Extension of AdvertiseOptions (untemplated).
  class AdvertiseOptionsExt
    : public AdvertiseOptions
  {
    public:
      /// Just calls the parent constructor.
      AdvertiseOptionsExt();

      /**
       * @brief Intialize AdvertiseOptions with \a rt.
       * Does nothing if rt is NULL.
       */
      void initWithRosTopic(ROSBinding::ROSTopic* rt);
  };


  /**
   * @brief Allows us to define callbacks on new message.
   * SubscriptionCallbackHelper is abstract, so we need to redefine a lot of
   * methods. The version used by ROS is templated, that's why we need to
   * redefine our own class.
   */
  class SubscriptionCallbackHelperExt
    : public SubscriptionCallbackHelper
  {
    public:
      /// Type of our callback function (returns void and take a UValuePtr).
      typedef const boost::function<void(ROSBinding::UValuePtr)> Callback;
      /// Type of our deserialization function.
      typedef const boost::function<urbi::UValue*(unsigned char*, unsigned int)> SerializerCB;

      /**
       * @brief Constructor
       * Takes a callback (\a deserializer) for deserialization,
       * and an other callback (\a callback) for call with deserialized object.
       */
      SubscriptionCallbackHelperExt(Callback callback, SerializerCB deserializer,
                                    bool fixed_size = true);

      /// Empty destructor.
      virtual ~SubscriptionCallbackHelperExt();

      /**
       * @brief Deserialize using buffer in \a p.
       * @return a shared void* pointer to our UValue.
       */
      virtual VoidConstPtr deserialize(
          const SubscriptionCallbackHelperDeserializeParams& p);

      /// Call the callback_ with a UValuePtr forged from \a param.
      virtual void call(SubscriptionCallbackHelperCallParams& params);

      /// Needed by parent class, ROS centered, we return a dummy type.
      virtual const std::type_info& getTypeInfo();

      /// Returns true when the message type is fixed size.
      virtual bool isConst();


    private:
      /// Callback to our function that takes an UValuePtr
      Callback callback_;

      /// Callback to our deserialization function.
      SerializerCB deserializer_;

      /// Used in isConst().
      bool fixed_size_;
  };
}

#include "ros-overload.hxx"

#endif // ROS_OVERLOAD_HH

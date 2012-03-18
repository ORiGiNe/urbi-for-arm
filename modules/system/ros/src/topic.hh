/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef TOPIC_HH
# define TOPIC_HH

# include <libport/lockable.hh>
# include <urbi/uobject.hh>

namespace ROSBinding
{
  typedef boost::shared_ptr<urbi::UValue const> UValueConstPtr;
  typedef boost::shared_ptr<urbi::UValue> UValuePtr;

  class ROSTopic;
}

# include "tools.hh"
# include "ros-overload.hh"
# include "topic-msg.hh"

/// Size of the message queue for ROS (de)serializarion.
# define ROS_QUEUE_SIZE     100


namespace ROSBinding
{
  /**
   * @brief UObject hander for ROS topics (publisher and subscribers).
   */
  class ROSTopic
    : public urbi::UObject
  {
    public:
      /// C++ constructor for the uobject.
      ROSTopic(const std::string& name);

      /// C++ destructor.
      ~ROSTopic();

      /**
       * @brief Urbi Constructor (takes the name of the topic in \a topic).
       * @throw libport::Exception
       *      When already running as Subscriber or Publisher,
       *      if the topic name is invalid or empty [A-Za-z0-9_/-].
       */
      int init(const std::string& topic);


      /**
       * @brief Subscribe to the topic (type fetched automatically).
       * @throw libport::Exception
       *      If topic already defined as subscriber or publisher,
       *      if roscore is unreachable,
       *      if the topic is not registered.
       */
      void subscribe();

      /**
       * @brief Unsubscribe from a previously subscribed topic.
       * Does nothing if the UObject is not registered as a subscriber.
       */
      void unsubscribe();


      /**
       * @brief Advertise on a topic, with type \a msg_type.
       * If msg_type == "", then the function tries to find whether the topic
       * is already published and to get its type.
       * @throw libport::Exception
       *      If topic already defined as subscriber or publisher,
       *      if roscore is unreachable,
       *      if the type in \a msg_type does not exist,
       *      if the topic is already registered with a different type.
       */
      void advertise(std::string msg_type);

      /**
       * @brief Unadvertise from a previously published topic.
       * Does nothing if the UObject is not registered as an emitter.
       */
      void unadvertise();


      /**
       * @brief Publish the message \a pl on the previously advertised topic.
       * @throw if the UObject is not an emitter (or internal error).
       */
      void publish(urbi::UDictionary pl);


      /**
       * @brief Get the number of subscribers on the current topic.
       * @throw libport::Exception when roscore is unreachable.
       */
      int subscriber_number_get() const;


      /// \name ROS Callbacks for publishers (inst_type_ is TYPE_EMITTER).
      /// \{
      /// Called when a node subscribes (need to be public because of ROS).
      void subscriber_connect(const ros::SingleSubscriberPublisher& sub);
      /// Called when a node unsubscribes (need to be public because of ROS).
      void subscriber_disconnect(const ros::SingleSubscriberPublisher& sub);
      /// \}

      /**
       * @brief Called on every mesage received by the Subscriber.
       * Only when instance is a subscriber (inst_type_ is TYPE_SUBSCRIBER),
       * public because of ROS API.
       */
      void msg_callback(UValuePtr msg);


      /**
       * @brief Contains template structure for the current topic
       * Only avaible when inst_type_ is TYPE_EMITTER of TYPE_SUBSCRIBER.
       */
      urbi::UVar structure;

      /// Event triggered on new message (inst_type_ is TYPE_SUSCRIBER).
      urbi::UEvent ev_msg;

      /// Event triggered on new subscriber (inst_type_ is TYPE_EMITTER).
      urbi::UEvent sub_connect;

      /// Event triggered on subscriber leave (inst_type_ is TYPE_EMITTER).
      urbi::UEvent sub_disconnect;


      /// Get the current topic name.
      const std::string& topic_get() const;

      /// Get the instance of topicmsg (needed by ROS-Overload stuff).
      const TopicMsg* topic_msg_get() const;

      /// Getter for queue_size_ (not accessible through urbiscript).
      size_t queue_size_get() const;

      /// Setter for queue_size_ (not accessible through urbiscript).
      void queue_size_set(size_t size);


    private:
      /// Types of instances that this class handles.
      enum InstanceType {
        TYPE_NOT_DEFINED = 0,
        TYPE_SUBSCRIBER,
        TYPE_EMITTER
      };

      /// Name of the current topic.
      std::string topic_;

      // Number of elements to keep before dropping.
      size_t queue_size_;

      /// Type of instance (listener, emitter, or not (yet) defined).
      InstanceType inst_type_;

      /**
        * @brief Handle ROS topic type (internal structure of message, etc...).
        *
        * Shared between instances using the same type.
        */
      TopicMsg* topic_msg_;

      /// ROS Subscriber object.
      ros::Subscriber ros_sub_;

      /// ROS Publisher object.
      ros::Publisher ros_pub_;

      /// Mutex for event to prevent deadlock on unsubscription.
      libport::Lockable msg_lock_;
  };
}

#include "topic.hxx"

#endif // ! TOPIC_HH

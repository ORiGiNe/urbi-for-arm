/*
 * Copyright (C) 2007-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ROS_TOPIC_HXX
# define ROS_TOPIC_HXX

# include <libport/debug.hh>

namespace ROSBinding
{
  inline
  void
  ROSTopic::msg_callback(UValuePtr msg)
  {
    libport::BlockLock l(msg_lock_);
    ev_msg.emit(*msg);
  }


  inline
  const std::string&
  ROSTopic::topic_get() const
  {
    return topic_;
  }


  inline
  const TopicMsg*
  ROSTopic::topic_msg_get() const
  {
    return topic_msg_;
  }


  inline
  size_t
  ROSTopic::queue_size_get() const
  {
    return queue_size_;
  }

  inline
  void
  ROSTopic::queue_size_set(size_t size)
  {
    queue_size_ = size;
  }


  inline
  void
  ROSTopic::subscriber_connect(const ros::SingleSubscriberPublisher& sub)
  {
    sub_connect.emit(sub.getSubscriberName());
  }


  inline
  void
  ROSTopic::subscriber_disconnect(const ros::SingleSubscriberPublisher& sub)
  {
    sub_disconnect.emit(sub.getSubscriberName());
  }

}

#endif // ! ROS_TOPIC_HXX

/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ROS_OVERLOAD_HXX
# define ROS_OVERLOAD_HXX

namespace ros
{
  inline
  NodeHandleExt::NodeHandleExt(const std::string& ns)
    : NodeHandle(ns)
  {}

  inline
  SubscribeOptionsExt::SubscribeOptionsExt()
    : SubscribeOptions()
  {}

  inline
  AdvertiseOptionsExt::AdvertiseOptionsExt()
    : AdvertiseOptions()
  {}

  inline
  SubscriptionCallbackHelperExt::~SubscriptionCallbackHelperExt()
  {}

  inline
  bool
  SubscriptionCallbackHelperExt::isConst()
  {
    return fixed_size_;
  }
}

#endif // ROS_OVERLOAD_HXX

/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef TOOLS_HXX
# define TOOLS_HXX


/// Inform urbiscript constructor that it failed (as ROSTools is a singleton).
inline
int
ROSBinding::ROSTools::init()
{
  return 1;
}


inline
ROSBinding::ROSTools::NodeHandleExtPtr
ROSBinding::ROSTools::NodeHandle()
{
  return ros_nh_;
}


inline
ROSBinding::ROSTools*
ROSBinding::ROSTools::instance_get()
{
  return instance_;
}


inline
bool
ROSBinding::ROSTools::master_check() const
{
  return ros::master::check();
}


inline
std::string
ROSBinding::ROSTools::get_name() const
{
  return ros::this_node::getName();
}

#endif // ! TOOLS_HXX

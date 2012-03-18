/*
 * Copyright (C) 2010-2011, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <ros/ros.h>
#include <utopics/DoubleInt32.h>
#include <signal.h>
#include <stdio.h>


class SimpleReplay
{
public:
  SimpleReplay();
  void callback(const utopics::DoubleInt32Ptr& msg);

  private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
  size_t counter;
};


SimpleReplay::SimpleReplay()
  : counter(0)
{
  pub_ = nh_.advertise<utopics::DoubleInt32>("utests/replay", 5);
  sub_ = nh_.subscribe("/utests/play", 5, &SimpleReplay::callback, this);
}


void
SimpleReplay::callback(const utopics::DoubleInt32Ptr& msg)
{
  pub_.publish(*msg);
  if (++counter == 5)
  {
    ROS_DEBUG("simpleReplay: sent 5 double, shutting down ROS");
    ros::shutdown();
  }
}


void
quit(int sig)
{
  ros::shutdown();
  exit(0);
}


int
main(int argc, char** argv)
{
  ros::init(argc, argv, "simpleReplay");
  signal(SIGINT, quit);

  SimpleReplay instance;

  ros::spin();
  ROS_DEBUG("simpleReplay: done");
  return 0;
}

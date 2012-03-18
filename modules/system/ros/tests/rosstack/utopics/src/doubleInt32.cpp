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
#include <std_msgs/Bool.h>
#include <csignal>
#include <cstdio>


class DoubleInt32
{
public:
  DoubleInt32();
  void run(const std_msgs::BoolPtr& ready);

private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
};


DoubleInt32::DoubleInt32()
{
  pub_ = nh_.advertise<utopics::DoubleInt32>("utests/doubleInt32", 1);
  sub_ = nh_.subscribe("/ready", 5, &DoubleInt32::run, this);
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
  ros::init(argc, argv, "doubleInt32");
  signal(SIGINT, quit);

  DoubleInt32 instance;
  ros::spin();

  return 0;
}

void
DoubleInt32::run(const std_msgs::BoolPtr& ready)
{
  static int32_t valtable[10] = {42, 51, 5, 67, -1, 0, 65536, 8, 9, 10};
  utopics::DoubleInt32 d;

  for (int i = 0; i < 10; ++i)
  {
    d.a = valtable[i];
    d.b = -(valtable[i] & 0x3);

    pub_.publish(d);
    sleep(1);
  }
  ros::shutdown();
}


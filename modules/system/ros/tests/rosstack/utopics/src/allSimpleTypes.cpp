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
#include <utopics/AllSimpleTypes.h>
#include <std_msgs/Bool.h>
#include <signal.h>
#include <stdio.h>


class AllSimpleTypes
{
public:
  AllSimpleTypes();
  void run(const std_msgs::BoolPtr& ready);

private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
};


AllSimpleTypes::AllSimpleTypes()
{
  pub_ = nh_.advertise<utopics::AllSimpleTypes>("utests/allSimpleTypes", 1);
  sub_ = nh_.subscribe("/ready", 5, &AllSimpleTypes::run, this);
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
  ros::init(argc, argv, "allSimpleTypes");
  signal(SIGINT, quit);

  AllSimpleTypes instance;
  ros::spin();

  return 0;
}


void
AllSimpleTypes::run(const std_msgs::BoolPtr& ready)
{
  utopics::AllSimpleTypes d;
  d.level = 3;
  d.isTrue = true;
  d.message = "coin ";

  for (int i = 0; i < 3; ++i)
  {
    d.header.stamp = ros::Time::now();
    d.float64 = -105.0042;
    d.isTrue = !d.isTrue;
    d.message += "O<";
    // GCC 4.1 is buggy, don't assign a literal which requires more
    // than 32 bits here.
    d.bigint = 429496733;
    d.bigint *= 10;
    ++d.level;

    pub_.publish(d);
    sleep(1);
  }
  ros::shutdown();
}

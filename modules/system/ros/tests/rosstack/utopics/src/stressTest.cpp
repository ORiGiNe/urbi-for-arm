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
#include <utopics/BinaryData.h>
#include <std_msgs/Bool.h>
#include <signal.h>
#include <stdio.h>


class StressTest
{
public:
  StressTest();
  void run(const std_msgs::BoolPtr& ready);

private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
};


StressTest::StressTest()
{
  pub_ = nh_.advertise<utopics::BinaryData>("utests/stressTest", 1);
  sub_ = nh_.subscribe("/ready", 5, &StressTest::run, this);
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
  ros::init(argc, argv, "stressTest");
  signal(SIGINT, quit);

  StressTest instance;
  ros::spin();

  return 0;
}


void
StressTest::run(const std_msgs::BoolPtr& ready)
{
  ros::Rate loop_rate(50);
  utopics::BinaryData d;
  d.width = 640;
  d.height = 480;
  d.encoding = "rgb";

  size_t data_size = 640 * 480 * 3;
  d.data.resize(data_size);

  unsigned int count = 0;
  while (ros::ok())
  {
    memset(&d.data[0], 0, data_size);
    // A part of the image is grey.
    memset(&d.data[0] + 3 * 640 * (count / 2),
           0x33 + (count / 2), 640 * 2 * count);

    pub_.publish(d);

    loop_rate.sleep();
    ros::spinOnce();

    if (++count >= 350)
      break;
  }
  ros::shutdown();
}

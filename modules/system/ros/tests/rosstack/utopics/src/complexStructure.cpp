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
#include <utopics/ComplexStructure.h>
#include <std_msgs/Bool.h>
#include <signal.h>
#include <stdio.h>


class ComplexStructure
{
public:
  ComplexStructure();
  void run(const std_msgs::BoolPtr& ready);

private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  ros::Subscriber sub_;
};


ComplexStructure::ComplexStructure()
{
  pub_ = nh_.advertise<utopics::ComplexStructure>("utests/complexStructure", 1);
  sub_ = nh_.subscribe("/ready", 5, &ComplexStructure::run, this);
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
  ros::init(argc, argv, "complexStructure");
  signal(SIGINT, quit);

  ComplexStructure instance;
  ros::spin();

  return 0;
}


void
ComplexStructure::run(const std_msgs::BoolPtr& ready)
{
  utopics::ComplexStructure d;

  d.name = "root";
  d.port = 31337;
  d.messages.push_back("coin!");
  d.messages.push_back("meuh");
  d.robots[0] = "nao";
  d.robots[1] = "spykee";
  d.robots[2] = "LegoStorm NXT";

  utopics::ComplexSubStructure s;
  s.name = "node1";
  s.port = 4222;
  s.position[0] = -1.72;
  s.position[1] = 13.76;
  s.position[2] = 2.54;
  s.stuff.push_back("This is stuff!");

  d.coords[0].a = 4;
  d.coords[0].b = -8;
  d.coords[1].a = -15;
  d.coords[1].b = 16;

  d.nodes.push_back(s);

  pub_.publish(d);

  ros::shutdown();
}

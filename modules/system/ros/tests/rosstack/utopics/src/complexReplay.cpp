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
#include <signal.h>
#include <stdio.h>


class ComplexReplay
{
  public:
    ComplexReplay();
    void callback(const utopics::ComplexStructurePtr& msg);

  private:
    ros::NodeHandle nh_;
    ros::Publisher pub_;
    ros::Subscriber sub_;
    size_t counter;
};


ComplexReplay::ComplexReplay()
  : counter(0)
{
  pub_ = nh_.advertise<utopics::ComplexStructure>("utests/replay", 5);
  sub_ = nh_.subscribe("/utests/play", 5, &ComplexReplay::callback, this);
}


void
ComplexReplay::callback(const utopics::ComplexStructurePtr& msg)
{
  pub_.publish(*msg);
  if (++counter == 2)
    ros::shutdown();
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
  ros::init(argc, argv, "complexReplay");
  signal(SIGINT, quit);

  ComplexReplay instance;

  ros::spin();

  return 0;
}

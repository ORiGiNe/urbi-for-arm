/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef TOOLS_HH
# define TOOLS_HH

# include <ros/ros.h>
# include <urbi/uobject.hh>

namespace ros
{
  class NodeHandleExt;
}

/// Used to convert piece of urbiscript code into string.
# define TO_STRING(code)  # code
/// Defines the node name (ROS PoV).
# define ROS_NODE_NAME  "urbi"


/**
 * @brief Contains all classes related to ros.
 */
namespace ROSBinding
{
  /**
   * @brief Provide some tools to communicate with the ROS world through Urbi.
   *
   * ROSTools is an UObject, but also serves other ROSBinding classes by providing
   * methods to get the node name, or to check whether roscore is launched.
   */
  class ROSTools
    : public urbi::UObject
  {
    public:
      /// UObject Constructor.
      ROSTools(const std::string& name);

      /// Urbiscript constructor (useless here).
      int init();

      /// Initialize this ros node, the spinner, etc...
      int init_ros();

      /// Get the name of the current ROS Node.
      std::string get_name() const;

      /// Get the running nodes and some more information on each node.
      urbi::UDictionary get_nodes() const;

      /// Get the topics availables (and subscribers/publishers).
      urbi::UDictionary get_topics() const;

      /// Get the services availables (and their provider(s)).
      urbi::UDictionary get_services() const;

      /// Get the state of master (non blocking, true when master is started).
      bool master_check() const;


      /// Used when reference to a string is needed.
      static const std::string EMPTY_STRING;


    private:
      /// Our ROS NodeHandle reference (needed by ROS for each node).
      typedef boost::shared_ptr<ros::NodeHandleExt> NodeHandleExtPtr;

      /// Our ROS-Event watcher.
      typedef boost::shared_ptr<ros::AsyncSpinner> SpinnerPtr;

    public:
      /// Returns the ROS NodeHandle reference associated with this node.
      static NodeHandleExtPtr NodeHandle();

      /// Only one instance of ROSTools is authorized.
      static ROSTools* instance_get();

      /// Clean a topic / service name (provided in \a p).
      static std::string clean(const std::string& p);

    private:
      /// ROS NodeHandle needed by ROS for each node.
      static NodeHandleExtPtr ros_nh_;

      /// The current and only instance of ROSTools.
      static ROSTools* instance_;

      /// ROS event manager (handle the thread for ROS events).
      static SpinnerPtr ros_spinner_;

      /// In case ROS is not launched, retry every 200ms.
      urbi::TimerHandle timer_;
  };
}

# include "ros-overload.hh"
# include "tools.hxx"

#endif // ! TOOLS_HH

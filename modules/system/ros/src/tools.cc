/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/debug.hh>
#include <libport/exception.hh>
#include <libport/foreach.hh>

#include "tools.hh"


using ROSBinding::ROSTools;
using urbi::LOCK_NONE;
using libport::Exception;

GD_CATEGORY(Ros.Tools);


// We need sometime a reference to an empty string.
const std::string ROSTools::EMPTY_STRING = "";

// ROS global declarations about this node.
ROSTools::NodeHandleExtPtr ROSTools::ros_nh_ = ROSTools::NodeHandleExtPtr();
ROSTools::SpinnerPtr ROSTools::ros_spinner_ = ROSTools::SpinnerPtr();

// The current instance of ROSTools.
ROSTools* ROSTools::instance_ = NULL;


// Declare ROSTools as an UObject.
UStart(ROSTools);


ROSTools::ROSTools(const std::string& name)
  : urbi::UObject(name)
{
  UBindFunction(ROSTools, init);

  if (!instance_)
  {
    UBindThreadedFunctionRename(ROSTools, get_nodes, "nodes", LOCK_NONE);
    UBindThreadedFunctionRename(ROSTools, get_topics, "topics", LOCK_NONE);
    UBindThreadedFunctionRename(ROSTools, get_services, "services",
                                LOCK_NONE);
    UBindFunctionRename(ROSTools, master_check, "checkMaster");
    UBindFunctionRename(ROSTools, get_name, "name");

    char* argv[] = {(char*) "urbi", (char*) NULL};
    int argc = 1;
    ros::init(argc, argv, ROS_NODE_NAME, ros::init_options::NoSigintHandler  |
                                         ros::init_options::AnonymousName    |
                                         ros::init_options::NoRosout);
    instance_ = this;
    if (!init_ros())
    {
      GD_WARN("Unable to communicate with ROS master, retrying.");
      timer_ = USetTimer(1000, &ROSTools::init_ros);
    }

    call("System", "eval", TO_STRING(
      if (!Global.hasLocalSlot("Ros"))
        var Global.Ros = Object.new|
      __ros_tools: at (uobjects.slotAdded?)
      {
        if (uobjects.hasLocalSlot("ROSTools")
            && !Global.Ros.hasLocalSlot("checkMaster"))
        {
          for (var v : ["checkMaster", "name", "nodes", "services", "topics"])
            Global.Ros.setSlot(v, uobjects.ROSTools.getSlot(v));
          __ros_tools.block;
        }
      }|
      __ros_topic: at (uobjects.slotAdded?)
      {
        if (uobjects.hasLocalSlot("ROSTopic")
            && !Global.Ros.hasLocalSlot("Topic"))
        {
          var Global.Ros.Topic = uobjects.ROSTopic;
          __ros_topic.block;
        };
      }|
      __ros_service: at (uobjects.slotAdded?)
      {
        if (uobjects.hasLocalSlot("ROSService")
            && !Global.Ros.hasLocalSlot("Service"))
        {
          var Global.Ros.Service = uobjects.ROSService;
          __ros_service.block;
        };
      };
           ));
  }
}


int
ROSTools::init_ros()
{
  {
    static int n = 0;
    GD_FINFO_DUMP("ros::master::check (%s)", n);
    bool r = ros::master::check();
    GD_FINFO_DUMP("ros::master::check (%s) = %s", n, r);
    if (!r)
      return false;
  }

  ros_nh_ = NodeHandleExtPtr(new ros::NodeHandleExt());
  ros_spinner_ = SpinnerPtr(new ros::AsyncSpinner(0));
  ros_spinner_->start();

  if (timer_)
  {
    GD_INFO_TRACE("ROS Initialized");
    removeTimer(timer_);
  }

  return true;
}


/// Describe ROS Node properties.
static std::string s_states[3] =
{
  "publish",
  "subscribe",
  "services"
};

/// Describe ROS Topic properties.
static std::string t_states[3] =
{
  "publishers",
  "subscribers"
};


urbi::UDictionary
ROSTools::get_nodes() const
{
  if (!master_check())
    throw Exception("Master doesn't seem to be launched.");
  XmlRpc::XmlRpcValue args, res, payload;
  args[0] = ros::this_node::getName();
  if (!ros::master::execute("getSystemState", args, res, payload, true))
    throw Exception("Error while gettting system state.");
  if (payload.size() < 3)
    throw Exception("getSystemState: Incorrect size in the response.");

  urbi::UDictionary result;

  // Look for publishers.
  for (int s = 0; s < 3; ++s)
    for (int i = payload[s].size() - 1; i >= 0; --i)
      if (payload[s][i].size() >= 2)
      {
        std::string topic = (std::string) payload[s][i][0];
        for (int j = payload[s][i][1].size() - 1; j >= 0; --j)
        {
          std::string nodeName = (std::string) payload[s][i][1][j];
          if (result[nodeName].type == urbi::DATA_VOID)
          {
            result[nodeName] = urbi::UDictionary();
            urbi::UDictionary& d = *result[nodeName].dictionary;
            for (int k = 0; k < 3; ++k)
              d[s_states[k]] = urbi::UList();
          }
          (*result[nodeName].dictionary)[s_states[s]].list->push_back(topic);
        }
      }

  return result;
}


urbi::UDictionary
ROSTools::get_services() const
{
  if (!master_check())
    throw Exception("Master doesn't seem to be launched.");
  XmlRpc::XmlRpcValue args, res, payload;
  args[0] = ros::this_node::getName();
  if (!ros::master::execute("getSystemState", args, res, payload, true))
    throw Exception("Error while gettting system state");
  if (payload.size() < 3)
    throw Exception("getSystemState: Incorrect size in response.");

  urbi::UDictionary result;
  for (int i = payload[2].size() - 1; i >= 0; --i)
  {
    if (payload[2][i].size() >= 2)
    {
      result[payload[2][i][0]] = urbi::UList();
      urbi::UList* list = result[payload[2][i][0]].list;
      for (int j = payload[2][i][1].size() - 1; j >= 0; --j)
        list->push_back((std::string) payload[2][i][1][j]);
    }
  }
  return result;
}


urbi::UDictionary
ROSTools::get_topics() const
{
  if (!master_check())
    throw Exception("Master doesn't seem to be launched.");
  urbi::UDictionary result;
  ros::master::V_TopicInfo topic_info;
  if (!ros::master::getTopics(topic_info))
    throw Exception("Error while getting topics.");

  XmlRpc::XmlRpcValue args, res, payload;
  args[0] = ros::this_node::getName();
  if (!ros::master::execute("getSystemState", args, res, payload, true))
    throw Exception("Error while gettting system state.");
  if (payload.size() < 2)
    throw Exception("getSystemState: Incorrect size in response.");

  foreach(const ros::master::TopicInfo& item, topic_info)
  {
    result[item.name] = urbi::UDictionary();
    urbi::UDictionary& dict = *result[item.name].dictionary;
    dict["type"] = item.datatype;

    for (int s = 0; s < 2; ++s)
    {
      dict[t_states[s]] = urbi::UList();
      urbi::UList* list = dict[t_states[s]].list;
      for (int i = payload[s].size() - 1; i >= 0; --i)
        if (payload[s][i].size() >= 2 && payload[s][i][0] == item.name)
          for (int j = payload[s][i][1].size() - 1; j >= 0; --j)
            list->push_back((std::string) payload[s][i][1][j]);
    }
  }

  return result;
}


std::string
ROSTools::clean(const std::string& p)
{
  foreach (const char c, p)
    if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
          c == '_' || c == '/' || (c >= '0' &&  c <= '9')))
      return ROSTools::EMPTY_STRING;

  if (p[0] != '/')
    return "/" + p;
  else
    return p;
}

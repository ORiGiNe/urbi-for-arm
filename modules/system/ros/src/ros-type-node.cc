/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstring>
#include <libport/debug.hh>

#include "../../../algorithm/md5/md5-impl.hh"
#include "ros-type-node.hh"
#include "tools.hh"

using ROSBinding::ROSTypeNode;
using ROSBinding::ROSType;
using ROSBinding::ROSTools;
using libport::format;


ROSTypeNode::ROSTypeNode(const std::string& name)
  : name_(name)
  , type_(TYPE_OBJECT)
  , is_array_(false)
  , is_const_(false)
  , array_size_(-1)
  , type_size_(0)
  , parent_(NULL)
{}


namespace ROSBinding
{
  /// Association between ROS types, their names, and their size (in byte).
  static const struct
  {
    ROSType id;
    const char* type;
    size_t size;
  } s_ros_types[] = {
    {TYPE_OBJECT  , ""        , 0},
    {TYPE_BOOL    , "bool"    , 1},
    {TYPE_INT8    , "int8"    , 1},
    {TYPE_BYTE    , "byte"    , 1},
    {TYPE_UINT8   , "uint8"   , 1},
    {TYPE_CHAR    , "char"    , 1},
    {TYPE_INT16   , "int16"   , 2},
    {TYPE_UINT16  , "uint16"  , 2},
    {TYPE_INT32   , "int32"   , 4},
    {TYPE_UINT32  , "uint32"  , 4},
    {TYPE_INT64   , "int64"   , 8},
    {TYPE_UINT64  , "uint64"  , 8},
    {TYPE_FLOAT32 , "float32" , 4},
    {TYPE_FLOAT64 , "float64" , 8},
    {TYPE_TIME    , "time"    , 8},
    {TYPE_DURATION, "duration", 8},
    {TYPE_STRING  , "string"  , 0}
  };

  /// Number of elements in the table above.
  static const size_t s_ros_types_size =
    sizeof(s_ros_types) / sizeof(s_ros_types[0]);
}


std::string
ROSTypeNode::to_string() const
{
  std::string result;
  if (type_ != TYPE_OBJECT)
  {
    result += ROSTypeNode::type_to_str(type_);

    if (is_array_)
    {
      if (array_size_ > 0)
        result += format("[%d]", array_size_);
      else
        result += "[]";
    }
  }
  else
    // Surprisingly, arrays of substructures are not mentionned...
    result += md5sum_get();

  return result + " " + name_;
}


std::string
ROSTypeNode::textsum_get() const
{
  if (type_ != TYPE_OBJECT)
    return "";

  std::string result;
  bool isFirst = true;
  foreach (const ROSTypeNode* n, children_)
  {
    if (!isFirst)
      result += "\n";
    result += n->to_string();
    isFirst = false;
  }

  return result;
}


std::string
ROSTypeNode::md5sum_get() const
{
  // It doesn't make sense to compute a md5 on anything else but an object.
  if (type_ != TYPE_OBJECT)
    return "";

  MD5 checksum;
  bool isFirst = true;
  foreach (const ROSTypeNode* n, children_)
  {
    if (!isFirst)
      checksum.update("\n");
    checksum.update(n->to_string());
    isFirst = false;
  }

  return checksum.digest_get();
}


ROSType
ROSTypeNode::str_to_type(const std::string& type)
{
  // > 0 is intended, TYPE_OBJECT is a special type.
  for (int i = s_ros_types_size - 1; i > 0; --i)
    if (type == s_ros_types[i].type)
      return s_ros_types[i].id;

  return TYPE_OBJECT;
}


urbi::UValue
ROSTypeNode::default_value_for(ROSType id)
{
  urbi::UValue result;

  if (id == TYPE_STRING)
    result = "";
  // All remaining types are integers or equivalent.
  else if (id == TYPE_DURATION || id == TYPE_TIME)
  {
    urbi::UList list;
    list.push_back(urbi::UValue(0));
    list.push_back(urbi::UValue(0));
    result = list;
  }
  else if (id != TYPE_OBJECT)
    result = 0;
  return result;
}


// Direct access by id for better performances.
std::string
ROSTypeNode::type_to_str(ROSType id)
{
  if ((size_t) id < s_ros_types_size)
    return s_ros_types[id].type;
  return "";
}


size_t
ROSTypeNode::type_size(ROSType id)
{
  if ((size_t) id < s_ros_types_size)
    return s_ros_types[id].size;
  return 0;
}


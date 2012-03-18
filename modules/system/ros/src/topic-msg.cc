/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#include <libport/cstdint>
#include <libport/debug.hh>
#include <libport/exception.hh>

#include "topic-msg.hh"

GD_CATEGORY(Ros.Topic.Msg);

using ROSBinding::TopicMsg;
using ROSBinding::ROSTypeNode;
using ROSBinding::ROSType;
using libport::Exception;
using libport::format;


TopicMsg::TopicMsg(const std::string& type)
  : CommonMsg(type)
{
  if (!type_check())
    throw Exception(libport::format("The type %s does not exist.", type));

  msg_.node_tree_create();
  md5sum_ = msg_.node_->md5sum_get();
}


TopicMsg::TopicMsg(const TopicMsg& t)
  : CommonMsg(t)
  , msg_(t.msg_)
{
  msg_.node_tree_create();
  md5sum_ = msg_.node_->md5sum_get();
}


TopicMsg&
TopicMsg::operator=(const TopicMsg& t)
{
  msg_ = t.msg_;
  msg_.node_tree_create();
  md5sum_ = msg_.node_->md5sum_get();
  return *this;
}


TopicMsg::~TopicMsg()
{}


bool
TopicMsg::type_check()
{
  std::string command = "rosmsg show " + type_ + " 2>/dev/null";
  msg_.data_.clear();

  FILE* f_typout = popen(command.c_str(), "r");
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, f_typout))
    if (buffer[0] && buffer[0] != '\n' && buffer[0] != '\r')
      msg_.data_.push_back(buffer);

  pclose(f_typout);

  if (msg_.data_.empty())
    return false;

  return true;
}


ros::SerializedMessage
TopicMsg::serialize(urbi::UDictionary* d) const
{
  if (!msg_.full_size_ || !msg_.is_fixed_size_)
    msg_.full_size_calculate(*d);

  ros::SerializedMessage m;
  m.num_bytes = msg_.full_size_ + sizeof(uint32_t);
  m.buf.reset(new uint8_t[m.num_bytes]);
  memcpy(m.buf.get(), &msg_.full_size_, sizeof(uint32_t));
  m.message_start = m.buf.get() + sizeof(uint32_t);

  uint32_t size = msg_.full_size_;
  uint8_t* buffer = m.message_start;
  msg_.serialize(buffer, size, *d);
  return m;
}

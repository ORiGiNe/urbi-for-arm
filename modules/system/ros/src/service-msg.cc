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

#include "../../../algorithm/md5/md5-impl.hh"
#include "service-msg.hh"

GD_CATEGORY(Ros.ServiceMsg);

using ROSBinding::ServiceMsg;
using ROSBinding::ROSTypeNode;
using ROSBinding::ROSType;
using libport::Exception;
using libport::format;


ServiceMsg::ServiceMsg(const std::string& type)
  : CommonMsg(type)
{
  if (!type_check())
    throw Exception(libport::format("The type %s does not exist.", type));

  req_.node_tree_create();
  res_.node_tree_create();

  MD5 checksum;
  checksum.update(req_.node_->textsum_get());
  checksum.update(res_.node_->textsum_get());
  md5sum_ = checksum.digest_get();
}


ServiceMsg::ServiceMsg(const ServiceMsg& s)
  : CommonMsg(s)
  , req_(s.req_)
  , res_(s.res_)
{
  req_.node_tree_create();
  res_.node_tree_create();

  MD5 checksum;
  checksum.update(req_.node_->textsum_get());
  checksum.update(res_.node_->textsum_get());
  md5sum_ = checksum.digest_get();
}


ServiceMsg&
ServiceMsg::operator=(const ServiceMsg& s)
{
  req_ = s.req_;
  res_ = s.res_;
  req_.node_tree_create();
  res_.node_tree_create();
  md5sum_ = s.md5sum_;
  return *this;
}


ServiceMsg::~ServiceMsg()
{}


bool
ServiceMsg::type_check()
{
  std::string command = "rossrv show " + type_ + " 2>/dev/null";
  req_.data_.clear();
  res_.data_.clear();

  bool isRequest = true;
  FILE* f_typout = popen(command.c_str(), "r");
  char buffer[BUFFER_SIZE];
  while (fgets(buffer, BUFFER_SIZE, f_typout))
  {
    if (std::string("---\n") == buffer)
      isRequest = false;
    else if (buffer[0] && buffer[0] != '\n')
    {
      if (isRequest)
        req_.data_.push_back(buffer);
      else
        res_.data_.push_back(buffer);
    }
  }

  pclose(f_typout);

  if (isRequest)
    return false;

  return true;
}


ros::SerializedMessage
ServiceMsg::serialize(urbi::UDictionary* d) const
{
  if (!req_.full_size_ || !req_.is_fixed_size_)
    req_.full_size_calculate(*d);

  ros::SerializedMessage m;
  m.num_bytes = req_.full_size_ + sizeof(uint32_t);
  m.buf.reset(new uint8_t[m.num_bytes]);
  memcpy(m.buf.get(), &req_.full_size_, sizeof(uint32_t));
  m.message_start = m.buf.get() + sizeof(uint32_t);

  uint32_t size = req_.full_size_;
  uint8_t* buffer = m.message_start;
  req_.serialize(buffer, size, *d);
  if (0 < size)
    GD_FERROR("Serialization was shorter by %d than expected.", size);
  return m;
}

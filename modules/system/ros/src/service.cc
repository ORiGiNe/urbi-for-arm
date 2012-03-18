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
#include <libport/thread.hh>
#include <ros/ros.h>
#include <ros/service_manager.h>
#include <ros/service_publication.h>

#include "service.hh"

using ROSBinding::ROSService;
using libport::Exception;
using libport::Socket;

GD_CATEGORY(Ros.Service);

// Declare our UObject to Urbi.
UStart(ROSService);


ROSService::ROSService(const std::string& name)
  : urbi::UObject(name)
  , persistent_(false)
  , socket_(new ServiceTypeSocket(this))
{
  UBindFunctionRename(ROSService, service_get, "name");

  UBindThreadedFunction(ROSService, init, urbi::LOCK_INSTANCE);
  UBindThreadedFunction(ROSService, request, urbi::LOCK_INSTANCE);

  UBindVarRename(ROSService, req_struct, "reqStruct");
  UBindVarRename(ROSService, res_struct, "resStruct");
  UBindVarRename(ROSService, initialized_, "initialized");
}


int
ROSService::init(const std::string& service, bool persistent)
{
  if (service.empty())
    throw Exception("Service name cannot be empty.");

  service_ = ROSTools::clean(service);
  if (service_.empty())
    throw Exception(libport::format("Invalid character in %s", service));

  std::string host;
  unsigned int port;
  if (!ros::ServiceManager::instance()->lookupService(service, host, port))
    throw Exception("This service is not registered.");

  persistent_ = persistent;
  initialized_ = false;
  // Response is asynchronous, see init_continue.
  type_get(host, port);

  return 0;
}


void
ROSService::init_continue(const std::string& type)
{
  if ((bool) initialized_)
  {
    GD_INFO_TRACE("already initialized");
    return;
  }

  service_msg_ = ServiceMsg::fetch(type);

  res_struct = service_msg_->res_struct_get();
  req_struct = service_msg_->req_struct_get();

  ros::M_string headers;
  headers["probe"] = "1";
  headers["callerid"] = ros::this_node::getName();

  ros_client_ = ros::ServiceClient(service_, persistent_, headers,
                                   service_msg_->md5sum_get());
  initialized_ = true;
  GD_INFO_TRACE("initialized");
  socket_->destroy();
}


ROSService::~ROSService()
{
  ros_client_ = ros::ServiceClient();
}


urbi::UDictionary
ROSService::request(urbi::UDictionary req)
{
  if (!initialized_)
    throw Exception("The initialization is not finished.");

  urbi::UDictionary* d = new urbi::UDictionary;
  req.swap(*d);
  ros::SerializedMessage req_m = service_msg_->serialize(d);
  ros::SerializedMessage res_m;
  if (!ros_client_.call(req_m, res_m, service_msg_->md5sum_get()))
    throw Exception("Request failed (is the service still advertised?).");

  UValuePtr resp =
    UValuePtr(service_msg_->deserialize(res_m.message_start, res_m.num_bytes));
  urbi::UDictionary res = *resp->dictionary;
  delete d;
  return res;
}


void
ROSService::type_get(const std::string& host, unsigned int port)
{
  if (boost::system::error_code erc = socket_->connect(host, port))
  {
    GD_FERROR("Boost: error %d while connecting socket", erc.message());
    return;
  }

  headers_type headers;
  headers["service"] = service_;
  headers["probe"] = "1";
  headers["callerid"] = ros::this_node::getName();
  headers["md5sum"] = "*";

  uint32_t size = 0;
  uint8_t* buffer = 0;

  foreach (const headers_type::value_type& arg, headers)
    size += sizeof(uint32_t) + arg.first.size() + arg.second.size()  + 1;

  buffer = new uint8_t[size + sizeof size];
  uint8_t* ptr = buffer;
  memcpy(ptr, &size, sizeof size);
  ptr += sizeof size;
  foreach (const headers_type::value_type& arg, headers)
  {
    uint32_t localsize = arg.first.size() + arg.second.size() + 1;
    memcpy(ptr, &localsize, sizeof localsize);
    ptr += sizeof localsize;
    memcpy(ptr, &arg.first[0], arg.first.size());
    ptr += arg.first.size();
    *ptr++ = '=';
    memcpy(ptr, &arg.second[0], arg.second.size());
    ptr += arg.second.size();
  }

  socket_->write(buffer, size + sizeof size);

  delete[] buffer;
}


size_t
ROSService::ServiceTypeSocket::onRead(const void* data, size_t length)
{
  size_t size = length;

  headers_type headers;

  uint32_t content_size = *(uint32_t*) data;
  data = (const char*) data + sizeof content_size;
  size -= sizeof content_size;

  while (size > 0)
  {
    uint32_t local_size = *(uint32_t*) data;
    size -= sizeof local_size;
    data = (const char*) data + sizeof local_size;

    // Prevent bad things to happen.
    if (size < local_size)
      break;

    std::string arg((const char*) data, local_size);
    size_t cut_position = arg.find('=');
    if (cut_position != std::string::npos)
    {
      std::string key = arg.substr(0, cut_position);
      ++cut_position;
      headers[key] = arg.substr(cut_position, local_size - cut_position);
    }

    size -= local_size;
    data = (const char*) data + local_size;
  }

  if (headers.find("type") != headers.end())
  {
    try
    {
      rs_->init_continue(headers["type"]);
    }
    catch (const Exception& e)
    {
      GD_FERROR("Exception caught: %s", e.what());
    }
  }

  return length - size;
}

/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ROS_SERVICE_HXX
# define ROS_SERVICE_HXX

#include <libport/debug.hh>

namespace ROSBinding
{

  /*-------------.
  | ROSService.  |
  `-------------*/

  inline
  const std::string&
  ROSService::service_get() const
  {
    return service_;
  }

  /*--------------------------------.
  | ROSService::ServiceTypeSocket.  |
  `--------------------------------*/

  inline
  ROSService::ServiceTypeSocket::ServiceTypeSocket(ROSService* rs)
    : super_type(rs->getIoService())
    , rs_(rs)
  {
    GD_CATEGORY(Ros.Service);
    GD_FINFO_TRACE("%p->ServiceTypeSocket::ServiceTypeSocket", this);
  }

  inline
  ROSService::ServiceTypeSocket::~ServiceTypeSocket()
  {
    GD_CATEGORY(Ros.Service);
    GD_FINFO_TRACE("%p->~ServiceTypeSocket", this);
  }

  inline
  void
  ROSService::ServiceTypeSocket::doDestroy()
  {
    GD_CATEGORY(Ros.Service);
    GD_FINFO_TRACE("%p->doDestroy", this);
    rs_->socket_ = 0;
    super_type::doDestroy();
  }

}

#endif // ! ROS_SERVICE_HXX

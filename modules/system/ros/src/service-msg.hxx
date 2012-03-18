/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SERVICE_MSG_HXX
# define SERVICE_MSG_HXX

inline
urbi::UValue
ROSBinding::ServiceMsg::req_struct_get() const
{
  return req_.build_structure();
}


inline
urbi::UValue
ROSBinding::ServiceMsg::res_struct_get() const
{
  return res_.build_structure();
}


inline
urbi::UValue*
ROSBinding::ServiceMsg::deserialize(const unsigned char* buffer,
                                    unsigned int size) const
{
  return res_.deserialize(buffer, size);
}


#endif // ! SERVICE_MSG_HXX

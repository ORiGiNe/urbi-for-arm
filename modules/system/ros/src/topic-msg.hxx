/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef TOPIC_MSG_HXX
# define TOPIC_MSG_HXX

inline
urbi::UValue
ROSBinding::TopicMsg::structure_get() const
{
  return msg_.build_structure();
}


inline
urbi::UValue*
ROSBinding::TopicMsg::deserialize(const unsigned char* buffer,
                                  unsigned int size) const
{
  return msg_.deserialize(buffer, size);
}


inline
bool
ROSBinding::TopicMsg::has_header() const
{
  return msg_.has_header_;
}


inline
bool
ROSBinding::TopicMsg::is_fixed_size() const
{
  return msg_.is_fixed_size_;
}

#endif // ! TOPIC_MSG_HXX

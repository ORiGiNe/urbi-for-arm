/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef COMMON_MSG_HXX
# define COMMON_MSG_HXX

namespace ROSBinding
{
  inline
  uint32_t
  CommonMsg::strlen(const urbi::UValue& v)
  {
    if (v.type == urbi::DATA_STRING)
      return v.stringValue->size();
    throw libport::Exception("Found key but not a string as expected.");
  }


  inline
  const std::string&
  CommonMsg::type_get() const
  {
    return type_;
  }

  inline
  const std::string&
  CommonMsg::md5sum_get() const
  {
    return md5sum_;
  }


  inline
  urbi::UValue
  CommonMsg::ROSMsg::build_structure() const
  {
    return build_structure(&*node_);
  }


  inline
  void
  CommonMsg::ROSMsg::serialize(uint8_t*& buffer,
                               uint32_t& length,
                               const urbi::UDictionary& d) const
  {
    serialize_tree(buffer, length, *node_, d);
  }


  inline
  urbi::UValue*
  CommonMsg::ROSMsg::deserialize(const unsigned char* buffer,
                                 unsigned int size) const
  {
    urbi::UValue* result = deserialize_tree(buffer, size, *node_);
    return result;
  }


  inline
  uint32_t
  CommonMsg::ROSMsg::full_size_calculate(const urbi::UValue& v) const
  {
    full_size_ = full_size_get(*node_, v);
    return full_size_;
  }


  inline
  uint32_t
  CommonMsg::ROSMsg::full_size_calculate(const urbi::UDictionary& d) const
  {
    full_size_ = full_size_get(*node_, d);
    return full_size_;
  }


  inline
  uint32_t
  CommonMsg::ROSMsg::full_size_get(const ROSTypeNode& node,
                                   const urbi::UValue& v)
  {
    if (v.type != urbi::DATA_DICTIONARY)
      throw libport::Exception(
              libport::format("Item %s is not a dictionary.", node.name_get()));
    return full_size_get(node, *v.dictionary);
  }
}

#endif // ! COMMON_MSG_HXX

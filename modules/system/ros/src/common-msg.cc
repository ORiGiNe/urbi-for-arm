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
#include <libport/format.hh>

#include "common-msg.hh"

using ROSBinding::CommonMsg;
using libport::Exception;
using libport::format;

GD_CATEGORY(Ros.Msg);

CommonMsg::CommonMsg(const std::string& type)
  : type_(type)
{}

CommonMsg::ROSMsg::ROSMsg()
  : has_header_(false)
  , is_fixed_size_(true)
  , full_size_(0)
{}

uint32_t
CommonMsg::ROSMsg::full_size_get(const ROSTypeNode& root,
                                 const urbi::UDictionary& d)
{
  uint32_t result = 0;

  foreach(const ROSTypeNode* nn, root.children_get())
  {
    const ROSTypeNode& n = *nn;
    uint32_t coef = 1;

    if (n.is_const_get())
      continue;

    // Get the number in the list.
    if (n.is_array_get())
    {
      if (n.array_size_get() > 0)
        coef = n.array_size_get();
      else
      {
        // Because of prepend of size in case of a variable array.
        result += sizeof(uint32_t);
        try
        {
          bool is8bits = n.type_get() == TYPE_CHAR || n.type_get() == TYPE_UINT8
                         || n.type_get() == TYPE_INT8 || n.type_get() == TYPE_BYTE;
          coef = CommonMsg::size_find(d.at(n.name_get()), is8bits, true);
        }
        catch (std::out_of_range& e)
        {
          throw Exception(format("No key %s in dictionary.", n.name_get()));
        }
      }
    }

    if (n.type_get() == TYPE_OBJECT)
    {
      try
      {
        if (!n.is_array_get())
          result += full_size_get(n, d.at(n.name_get()));
        else
        {
          // List of dictionaries.
          const urbi::UValue& v = d.at(n.name_get());
          uint32_t effective_size = CommonMsg::size_find(v);
          if (effective_size < coef)
            throw Exception(format("Got a list of %d items, expected %d",
                                   effective_size, coef));
          foreach (const urbi::UValue* item, v.list->array)
            result += full_size_get(n, *item);
        }
      }
      catch (std::out_of_range& e)
      {
        throw Exception(format("Item %s doesn't exist in dictionary.",
              n.name_get()));
      }
    }
    else if (n.type_get() == TYPE_STRING)
    {
      try
      {
        if (!n.is_array_get())
        {
          result += sizeof(uint32_t);
          result += CommonMsg::strlen(d.at(n.name_get()));
        }
        else
        {
          // Array of strings.
          const urbi::UValue& v = d.at(n.name_get());
          uint32_t effective_size = CommonMsg::size_find(v);
          if (effective_size != coef)
            throw Exception(format("Got a list of %d items, expected %d",
                                   effective_size, coef));
          result += effective_size * sizeof(uint32_t);
          foreach (const urbi::UValue* item, v.list->array)
            result += CommonMsg::strlen(*item);
        }
      }
      catch (std::out_of_range& e)
      {
        throw Exception(format("Item %s doesn't exist in dictionary.",
                               n.name_get()));
      }
    }
    else
      result += coef * n.type_size_get();
  }

  return result;
}

uint32_t
CommonMsg::size_find(const urbi::UValue& v, bool is8bits, bool array)
{
  switch(v.type)
  {
    case urbi::DATA_LIST:
      return v.list->size();
      break;
    case urbi::DATA_DICTIONARY:
      return v.dictionary->size();
      break;
    case urbi::DATA_BINARY:
      return v.binary->common.size / (array ? sizeof(double) : 1);
      break;
    default:
      throw Exception(format("found key but not a list/binary as expected. %s "
                         "found instead.", v));
  }
}


urbi::UValue
CommonMsg::ROSMsg::build_structure(const ROSTypeNode* node)
{
  if (node->type_get())
    return urbi::UValue();

  urbi::UDictionary result;
  foreach (const ROSTypeNode* nn, node->children_get())
  {
    const ROSTypeNode& n = *nn;
    // We don't care about constants, only kept for md5 purpose.
    if (n.is_const_get())
      continue;
    urbi::UValue value;
    if (n.type_get() == TYPE_OBJECT)
    {
      value = build_structure(&n);

      if (n.is_array_get())
      {
        int count = n.array_size_get();
        if (count < 1)
          result[n.name_get()] = urbi::UList();
        else
        {
          urbi::UList list;
          for (int i = n.array_size_get(); i > 0; --i)
            list.push_back(value);
          result[n.name_get()] = list;
        }
      }
      else
        result[n.name_get()] = value;
    }
    else
    {
      value = ROSTypeNode::default_value_for(n.type_get());
      if (!n.is_array_get())
        // This atom is not an array.
        result[n.name_get()] = value;
      else if (n.array_size_get() > 0)
      {
        // The size of this array is fixed.
        urbi::UList array;
        for (int i = n.array_size_get(); i > 0; --i)
          array.push_back(value);
        result[n.name_get()] = array;
      }
      else
        result[n.name_get()] = urbi::UList();
    }
  }

  return urbi::UValue(result);
}


#define SERIALIZE_PTR(ptr, size)                    \
  do {                                              \
    if (size > length)                              \
      throw Exception("Size Overrun.");             \
    memcpy(buffer, ptr, size);                      \
    buffer += size;                                 \
    length -= size;                                 \
  } while (0)

#define CASE_HANDLE(type, ctype)                    \
    case type:                                      \
      SERIALIZE(ctype);                             \
      break

#define SERIALIZE(type)                             \
  {                                                 \
    type tmp = (type) dValue;                       \
    SERIALIZE_PTR(&tmp, sizeof(type));              \
  }

inline void serialize_scalar(uint8_t*& buffer, uint32_t& length, double dValue, ROSBinding::ROSType type)
{
  using namespace ROSBinding;
  switch (type)
  {
    case TYPE_INT8:
    CASE_HANDLE(TYPE_BYTE, int8_t);

    case TYPE_CHAR:
    case TYPE_BOOL:
    CASE_HANDLE(TYPE_UINT8, uint8_t);

    CASE_HANDLE(TYPE_INT16, int16_t);

    CASE_HANDLE(TYPE_UINT16, uint16_t);

    CASE_HANDLE(TYPE_INT32, int32_t);

    CASE_HANDLE(TYPE_UINT32, uint32_t);

    CASE_HANDLE(TYPE_INT64, int64_t);

    CASE_HANDLE(TYPE_UINT64, uint64_t);

    CASE_HANDLE(TYPE_FLOAT32, float);

    CASE_HANDLE(TYPE_FLOAT64, double);

    /// Fix a misleading compilation warning.
    default:
      ;
  }

}

void
CommonMsg::ROSMsg::serialize_value(uint8_t*& buffer,
                                   uint32_t& length,
                                   const ROSTypeNode& root,
                                   const urbi::UValue& v) const
{
  if (root.type_get() == TYPE_OBJECT)
    throw Exception("Unexpected Object while serializing value.");

  if (root.type_get() == TYPE_STRING)
  {
    if (v.type != urbi::DATA_STRING)
      throw Exception(format("Urbi value of %s is not a string.",
                             root.name_get()));
    uint32_t size = v.stringValue->size();
    SERIALIZE_PTR(&size, sizeof(size));
    SERIALIZE_PTR(v.stringValue->c_str(), size);
    return;
  }

  if (v.type == urbi::DATA_BINARY)
  {
    urbi::UBinary& b = *v.binary;
    if (b.type == urbi::BINARY_UNKNOWN)
      SERIALIZE_PTR(b.common.data, b.common.size);
    else if (b.type == urbi::BINARY_IMAGE)
      SERIALIZE_PTR(b.image.data, b.image.size);
    else if (b.type == urbi::BINARY_SOUND)
      SERIALIZE_PTR(b.sound.data, b.sound.size);
    else
      throw Exception(format("Unexpected empty binary for %s.",
                             root.name_get()));
    return;
  }

  if (root.type_get() == TYPE_TIME || root.type_get() == TYPE_DURATION)
  {
    if (v.type != urbi::DATA_LIST)
      throw Exception(format("time/duration (%s), not a list.",
                             root.name_get()));
    uint32_t lsz = CommonMsg::size_find(v);
    uint32_t sec;
    uint32_t nsec;
    if (has_header_ && root.parent_const_get()
        && root.parent_const_get()->name_get() == "header"
        && lsz != 2)
    { // Automatically fill stamp.
      ros::Time time = ros::Time::now();
      sec = time.sec;
      nsec = time.nsec;
    }
    else
    {
      if (lsz != 2)
        throw Exception(format("time/duration (%s), list has incorrect size.",
                               root.name_get()));
      sec = (uint32_t) v.list->array[0]->val;
      nsec = (uint32_t) v.list->array[1]->val;
    }
    SERIALIZE_PTR(&sec, sizeof(sec));
    SERIALIZE_PTR(&nsec, sizeof(nsec));

    return;
  }

  if (v.type != urbi::DATA_DOUBLE)
    throw Exception(format("Found key %s but not a float as expected.",
                           root.name_get()));

  // Here are all the integer types.
  double dValue = v.val;
  serialize_scalar(buffer, length, dValue, root.type_get());
#undef CASE_HANDLE
#undef SERIALIZE
}


void
CommonMsg::ROSMsg::serialize_tree(uint8_t*& buffer,
                                  uint32_t& length,
                                  const ROSTypeNode& root,
                                  const urbi::UValue& v) const
{
  if (v.type != urbi::DATA_DICTIONARY)
    throw Exception(format("Item %s is not a dictionary.", root.name_get()));
  serialize_tree(buffer, length, root, *v.dictionary);
}


void
CommonMsg::ROSMsg::serialize_tree(uint8_t*& buffer,
                                  uint32_t& length,
                                  const ROSTypeNode& root,
                                  const urbi::UDictionary& d) const
{
  uint32_t coef = 1;
  foreach (const ROSTypeNode* nn, root.children_get())
  {
    const ROSTypeNode& n = *nn;
    if (n.is_const_get())
      continue;
    if (n.parent_const_get() != &root)
      throw std::runtime_error("Inconsistency in tree on " + root.name_get());
    if (length <= 0)
      throw Exception("Size dropped to zero.");

    try
    {
      const urbi::UValue& v = d.at(n.name_get());
      bool is8bits = n.type_get() == TYPE_CHAR || n.type_get() == TYPE_UINT8
                     || n.type_get() == TYPE_INT8 || n.type_get() == TYPE_BYTE;

      // in case of a dynamic array, prepend with the size.
      if (n.is_array_get())
      {
        if (n.array_size_get() > 0)
          coef = n.array_size_get();
        else
        {
          coef = CommonMsg::size_find(v, is8bits, true);
          SERIALIZE_PTR(&coef, sizeof(coef));
        }
      }

      if (!n.is_array_get())
      {
        if (n.type_get() != TYPE_OBJECT)
          serialize_value(buffer, length, n, v);
        else
          serialize_tree(buffer, length, n, v);
      }
      else
      {
        // List of items.
        uint32_t effective_size = n.array_size_get() <= 0
                                  ? coef
                                  : CommonMsg::size_find(v);
        if (effective_size < coef)
          throw Exception(format("Got a list of %d items, expected %d",
                effective_size, coef));

        if (is8bits)
          serialize_value(buffer, length, n, v);
        else
        {
          if (v.type == urbi::DATA_LIST)
          foreach (const urbi::UValue* item, v.list->array)
          {
            if (n.type_get() != TYPE_OBJECT)
              serialize_value(buffer, length, n, *item);
            else
              serialize_tree(buffer, length, n, *item);
          }
          else if (v.type == urbi::DATA_BINARY)
          {
            UPackedData<double> d = urbi::uvalue_caster<UPackedData<double> >()(const_cast<urbi::UValue&>(v));
            for (unsigned i=0; i<d.size(); ++i)
              serialize_scalar(buffer, length, d[i], n.type_get());
          }
          else
            throw Exception(format("Expected a list, got type %d", v.type));
        }
      }
    }
    catch (std::out_of_range& e)
    {
      throw Exception(format("No key %s in dictionary.", n.name_get()));
    }
  }
}
#undef SERIALIZE_PTR


urbi::UValue*
CommonMsg::ROSMsg::deserialize_value(const unsigned char*& buffer,
                                     unsigned int& size,
                                     const ROSTypeNode& node) const
{
  urbi::UValue* result = new urbi::UValue();
  if (node.type_get() == TYPE_OBJECT)
    return result;

  if (node.type_get() == TYPE_STRING)
  {
    size_t len = *(uint32_t*) buffer;
    buffer += sizeof(uint32_t);
    size -= sizeof(uint32_t);

    if (len > size)
    {
      GD_FERROR("Error while deserializing string %s, len(%d) > size(%d)",
                node.name_get(), len, size);
      size = 0;
      return result;
    }

    // Optimized.
    result->type = urbi::DATA_STRING;
    result->stringValue = new std::string((const char*) buffer, len);
    buffer += len;
    size -= len;
    return result;
  }

  if (node.type_get() == TYPE_DURATION || node.type_get() == TYPE_TIME)
  {
    result->type = urbi::DATA_LIST;
    result->list = new urbi::UList();
    result->list->push_back((double) *(uint32_t*) buffer);
    result->list->push_back((double) *(uint32_t*) (buffer + 4));
    return result;
  }

  result->type = urbi::DATA_DOUBLE;
  libport::ufloat& value = result->val;
  switch (node.type_get())
  {
    case TYPE_INT8:
    case TYPE_BYTE:
      value = *(int8_t*) buffer;
      break;

    case TYPE_UINT8:
    case TYPE_CHAR:
    case TYPE_BOOL:
      value = *(uint8_t*) buffer;
      break;

    case TYPE_INT16:
      value = *(int16_t*) buffer;
      break;

    case TYPE_UINT16:
      value = *(uint16_t*) buffer;
      break;

    case TYPE_INT32:
      value = *(int32_t*) buffer;
      break;

    case TYPE_UINT32:
      value = *(uint32_t*) buffer;
      break;

    case TYPE_INT64:
      value = *(int64_t*) buffer;
      break;

    case TYPE_UINT64:
      value = *(uint64_t*) buffer;
      break;

    case TYPE_FLOAT32:
      value = *(float*) buffer;
      break;

    case TYPE_FLOAT64:
      value = *(double*) buffer;
      break;

    default:
      return result;
  }

  return result;
}


urbi::UValue*
CommonMsg::ROSMsg::deserialize_tree(const unsigned char*& buffer,
                                    unsigned int& size,
                                    const ROSTypeNode& root) const
{
  // Performances Optimizations.
  urbi::UValue* result = new urbi::UValue();
  result->type = urbi::DATA_DICTIONARY;
  result->dictionary = new urbi::UDictionary();
  urbi::UDictionary& d_result = *result->dictionary;

  foreach(const ROSTypeNode* nn, root.children_get())
  {
    const ROSTypeNode& n = *nn;
    if (n.is_const_get())
      continue;
    size_t node_type_size = n.type_size_get();
    if ((node_type_size && size < node_type_size)
        || (!node_type_size && size <= node_type_size))
    {
      GD_ERROR("ROS Message received has incorrect length!");
      break;
    }

    uint32_t array_size;
    if (n.type_get() == TYPE_OBJECT)
    {
      d_result[n.name_get()] = urbi::UValue();
      if (!n.is_array_get())
      {
        d_result[n.name_get()].type = urbi::DATA_DICTIONARY;
        urbi::UValue* itemPtr = deserialize_tree(buffer, size, n);

        d_result[n.name_get()].dictionary = itemPtr->dictionary;
        itemPtr->dictionary = 0;
        // This delete does not free the dictionary as we still need it.
        delete itemPtr;
      }
      else
      {
        // Array of complex types.
        if (n.array_size_get() > 0)
          array_size = n.array_size_get();
        else
	{
          // Get the size of the array.
          array_size = *(uint32_t*) buffer;
          buffer += sizeof(uint32_t);
          size -= sizeof(uint32_t);
        }

        d_result[n.name_get()].type = urbi::DATA_LIST;
        d_result[n.name_get()].list = new urbi::UList();
        urbi::UList& list = *d_result[n.name_get()].list;
        list.array.reserve(array_size);
        for (size_t i = 0; i < array_size && size > 0; ++i)
          list.array.push_back(deserialize_tree(buffer, size, n));
      }
    }
    else if (!n.is_array_get())
    {
      urbi::UValue* itemPtr = deserialize_value(buffer, size, n);
      if (itemPtr->type == urbi::DATA_STRING)
      {
        // Optimized in case of large strings.
        d_result[n.name_get()].type = urbi::DATA_STRING;
        d_result[n.name_get()].stringValue = itemPtr->stringValue;
        itemPtr->stringValue = 0;
        delete itemPtr;
      }
      else
      {
        d_result[n.name_get()] = *itemPtr;
        size -= node_type_size;
        buffer += node_type_size;
      }
    }
    else
    {
      d_result[n.name_get()] = urbi::UValue();
      // Arrays of simple types.
      if (n.array_size_get() > 0)
        array_size = n.array_size_get();
      else
      {
        // Get the size of the array.
        array_size = *(uint32_t*) buffer;
        buffer += sizeof(uint32_t);
        size -= sizeof(uint32_t);
      }

      if ((n.type_get() == TYPE_UINT8
           || n.type_get() == TYPE_INT8
           || n.type_get() == TYPE_BYTE
           || n.type_get() == TYPE_CHAR) && array_size > ROS_MIN_BIN_SIZE)
      {
        d_result[n.name_get()].binary = new urbi::UBinary();
        d_result[n.name_get()].type = urbi::DATA_BINARY;

        urbi::UBinary& b = *d_result[n.name_get()].binary;
        b.common.data = malloc(array_size);
        if (!b.common.data)
          throw std::bad_alloc();
        b.common.size = array_size;
        b.type = urbi::BINARY_UNKNOWN;

        memcpy(b.common.data, buffer, array_size);
        buffer += array_size;
        size -= array_size;
      }
      else
      {
        d_result[n.name_get()].list = new urbi::UList();
        d_result[n.name_get()].type = urbi::DATA_LIST;

        urbi::UList& list = *d_result[n.name_get()].list;
        list.array.reserve(array_size);
        for (size_t i = 0;
            i < array_size && size >= node_type_size;
            size -= node_type_size, buffer += node_type_size, ++i)
        {
          list.array.push_back(deserialize_value(buffer, size, n));
        }
      }
    }
  }

  return result;
}


void
CommonMsg::ROSMsg::node_tree_create()
{
  node_ = ROSTypeNodePtr(new ROSTypeNode("root"));

  // Number of tabulations per level.
  static const unsigned int TAB_SIZE = 2;

  // Initialization.
  unsigned int curr_level = -1;
  ROSTypeNode* curr_node = &*node_;
  bool header_check = false;

  foreach (const std::string& line, data_)
  {
    unsigned int eff_level = 0;
    ROSTypeNode* node = new ROSTypeNode;

    // Calculate the current level of indentation.
    std::string::const_iterator i = line.begin();
    std::string::const_iterator i_end = line.end();
    for (;i != i_end && *i == ' '; ++i)
      ++eff_level;

    // Skip empty lines.
    if (i == i_end)
      continue;

    eff_level /= TAB_SIZE;

    if (eff_level > curr_level + 1)
      throw Exception("node_tree_create: Unexpected indentation level.");

    char buffer[BUFFER_SIZE];
    int k = 0;
    for (;i != i_end && *i != ' ' && k < BUFFER_SIZE && *i != '['; ++i)
      buffer[k++] = *i;
    buffer[k] = 0;

    // Line is malformed, ignore it.
    if (i == i_end)
      continue;

    std::string type(buffer);
    node->type_set(ROSTypeNode::str_to_type(type));

    // Array related stuff.
    if (*i == '[')
    {
      ++i;
      node->is_array_set(true);
      for (k = 0; i != i_end && isdigit(*i); ++i)
        buffer[k++] = *i;
      buffer[k] = 0;

      // Variable length array.
      if (k == 0)
      {
        node->array_size_set(-1);
        is_fixed_size_ = false;
      }
      else
        node->array_size_set(strtol(buffer, 0, 10));
    }

    for (;i != i_end && (*i == ' ' || *i == ']'); ++i)
      ;

    // Unexpected end of line, skip this line.
    if (i == i_end)
      break;

    k = 0;
    for (;i != i_end && k < BUFFER_SIZE
          && *i != ' ' && *i != '\n' && *i != '\r'; ++i)
    {
      // This node is a constant, keep it for MD5 purpose only.
      if (*i == '=')
        node->is_const_set(true);
      buffer[k++] = *i;
    }
    // Malformed line.
    if (!k)
      continue;
    buffer[k] = 0;

    node->name_set(buffer);

    // Determine has_header_ and is_fixed_size_.
    if (node->type_get() == TYPE_STRING && !node->is_const_get())
      is_fixed_size_ = false;

    if (!header_check && !node->is_const_get())
    {
      if (type == "Header" && node->name_get() == "header")
      {
        has_header_ = true;
        header_check = true;
      }
      else
        header_check = true;
    }

    // We have to return to the parent node of our new node.
    for (int i = curr_level - eff_level + 1; i > 0; --i)
    {
      curr_node = curr_node->parent_get();
      /* Oops, this node has no parent, this shouldn't happen,
         but let's place it under the root node. */
      if (!curr_node)
      {
        curr_node = &*node_;
        break;
      }
    }
    node->parent_set(curr_node);
    curr_node->children_get().push_back(node);
    curr_level = eff_level;
    curr_node = curr_node->children_get().back();
  } // FOREACH
}


// Debug purpose, dump the raw buffer with HEX / ASCII.
/*
static
void
dump(const unsigned char* buffer, size_t len)
{
  static char hex[16] = {'0','1','2','3','4','5','6','7',
                         '8','9','A','B','C','D','E','F'};
  GD_CATEGORY(TopicMsg);
  char buff[1024];
  char inter[256];
  for (unsigned int j = 0; j < len;)
  {
    int i = 0;
    int p = 0;
    for (int k = 0; k < 32 && j < len; ++k)
    {
      buff[i++] = hex[(buffer[j] >> 4) & 0xF];
      buff[i++] = hex[buffer[j] & 0xF];
      buff[i++] = ' ';
      if (buffer[j] >= ' ' && buffer[j] <= '~')
        inter[p++] = buffer[j];
      else
        inter[p++] = '.';

      ++j;
      if (j % 4 == 0)
        buff[i++] = ' ';
    }

    buff[i++] = ' ';
    for (int k = 0; k < p; ++k)
      buff[i++] = inter[k];
    buff[i] = 0;
    GD_INFO_TRACE(buff);
  }
}
*/

/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ROS_TYPE_NODE_HXX
# define ROS_TYPE_NODE_HXX


inline
std::vector<ROSBinding::ROSTypeNode*>&
ROSBinding::ROSTypeNode::children_get()
{
  return children_;
}


inline
const std::vector<ROSBinding::ROSTypeNode*>&
ROSBinding::ROSTypeNode::children_get() const
{
  return children_;
}


inline
const std::string&
ROSBinding::ROSTypeNode::name_get() const
{
  return name_;
}


inline
bool
ROSBinding::ROSTypeNode::is_const_get() const
{
  return is_const_;
}


inline
bool
ROSBinding::ROSTypeNode::is_array_get() const
{
  return is_array_;
}


inline
int
ROSBinding::ROSTypeNode::array_size_get() const
{
  return array_size_;
}

inline
size_t
ROSBinding::ROSTypeNode::type_size_get() const
{
  return type_size_;
}

inline
ROSBinding::ROSType
ROSBinding::ROSTypeNode::type_get() const
{
  return type_;
}


inline
ROSBinding::ROSTypeNode*
ROSBinding::ROSTypeNode::parent_get()
{
  return parent_;
}


inline
const ROSBinding::ROSTypeNode*
ROSBinding::ROSTypeNode::parent_const_get() const
{
  return parent_;
}


inline
void
ROSBinding::ROSTypeNode::name_set(const std::string& name)
{
  name_ = name;
}


inline
void
ROSBinding::ROSTypeNode::is_const_set(bool is_const)
{
  is_const_ = is_const;
}


inline
void
ROSBinding::ROSTypeNode::is_array_set(bool is_array)
{
  is_array_ = is_array;
}


inline
void
ROSBinding::ROSTypeNode::type_set(ROSType type)
{
  type_ = type;
  type_size_ = type_size(type);
}


inline
void
ROSBinding::ROSTypeNode::array_size_set(int array_size)
{
  array_size_ = array_size;
}


inline
void
ROSBinding::ROSTypeNode::parent_set(ROSTypeNode* parent)
{
  parent_ = parent;
}


#endif // ! ROS_TYPE_NODE_HXX

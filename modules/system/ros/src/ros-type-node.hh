/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef ROS_TYPE_NODE_HH
# define ROS_TYPE_NODE_HH

# include <vector>
# include <urbi/uobject.hh>

namespace ROSBinding
{
  /**
   * @brief All possible ROS Type for a variable.
   * TYPE_OBJECT is special as it describes any sub-structure.
   */
  enum ROSType
  {
    // Add stuff at the end in case operators like < are used on this.
    TYPE_OBJECT = 0,
    TYPE_BOOL,
    TYPE_INT8,
    TYPE_BYTE,
    TYPE_UINT8,
    TYPE_CHAR,
    TYPE_INT16,
    TYPE_UINT16,
    TYPE_INT32,
    TYPE_UINT32,
    TYPE_INT64,
    TYPE_UINT64,
    TYPE_FLOAT32,
    TYPE_FLOAT64,
    TYPE_TIME,
    TYPE_DURATION,
    TYPE_STRING
  };


  /// Defines a single node in our ROS Type tree representation.
  class ROSTypeNode
  {
    public:
      /**
       * @brief Constructor, with all default values for an empty node.
       * Values have to be defined individually with individuals accessors.
       */
      ROSTypeNode(const std::string& name = "");

      /**
       * @brief Get the list of children for the current node.
       * If the current node is not an object (TYPE_OBJECT),
       * (should) return an empty vector.
       */
      std::vector<ROSTypeNode*>& children_get();

      /**
       * @brief Get the list of children for the current node (const version).
       * If the current node is not an object (TYPE_OBJECT),
       * (should) return an empty vector.
       */
      const std::vector<ROSTypeNode*>& children_get() const;


      /// Get the name of the current node (variable).
      const std::string& name_get() const;

      /// Get whether or not the node is a constant.
      bool is_const_get() const;

      /// Get whether or not the node is an array.
      bool is_array_get() const;

      /// Get the type of the node.
      ROSType type_get() const;

      /// Size of the array (releveant if is_array_ == true), -1 if unknown.
      int array_size_get() const;

      /// Get the size of the current type_.
      size_t type_size_get() const;

      /**
       * @brief Get the parent node (NULL on the first node).
       * Don't free it or your program will die badly.
       */
      ROSTypeNode* parent_get();

      /**
       * @brief Get a constant pointer to the parent node (or to NULL).
       */
      const ROSTypeNode* parent_const_get() const;


      /// Set the name of this node with \a name.
      void name_set(const std::string& name);

      /// Set whether or not this node is a constant.
      void is_const_set(bool is_const);

      /// Set whether or not this node is an array.
      void is_array_set(bool is_array);

      /// Set the type of this node with \a type.
      void type_set(ROSType type);

      /// Set the size if the array.
      void array_size_set(int array_size);

      /// Set the parent node of this node.
      void parent_set(ROSTypeNode* parent);

      /// String summary for this node (used in MD5 calculation).
      std::string to_string() const;

      /**
       * @brief Get the md5 of the current type.
       * @return an empty string when type_ != TYPE_OBJECT.
       */
      std::string md5sum_get() const;

      /**
       * @brief Get the base text used to calculate the MD5.
       * @return an empty string when type_ != TYPE_OBJECT.
       */
      std::string textsum_get() const;

      /// Get an UValue containing the default value for the type \a id.
      static urbi::UValue default_value_for(ROSType id);

      /// Get the ROSType corresponding to the string \a type (in ROS world).
      static ROSType str_to_type(const std::string& type);

      /// Get the plain text corresponding to \a id.
      static std::string type_to_str(ROSType id);

      /**
       * @brief Get the size of the type \a id.
       * @return 0 when \a id is TYPE_STRING or TYPE_OBJECT.
       */
      static size_t type_size(ROSType id);


    private:
      /// Name of the ROS variable.
      std::string name_;

      /// Type of data contained (TYPE_OBJECT when not a leaf).
      ROSType type_;

      /// True when the real type is an array.
      bool is_array_;

      /// True in case of a ROS constant
      bool is_const_;

      /// Size of the array (if releavant), -1 for variable length array.
      int array_size_;

      /**
       * @brief Used as a cache for deserialization.
       * Prevent subsequents calls to method type_size.
       */
      int type_size_;

      /// List of children (or empty when node is a leaf).
      std::vector<ROSTypeNode*> children_;

      /// Parent Node (no need to free it since it is a "reference").
      ROSTypeNode* parent_;
  };
}

#include "ros-type-node.hxx"

#endif // ! ROS_TYPE_NODE_HH

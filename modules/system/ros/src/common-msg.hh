/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef COMMON_MSG_HH
# define COMMON_MSG_HH

# include <libport/exception.hh>
# include <ros/ros.h>
# include <urbi/uobject.hh>
# include <vector>

# include "ros-type-node.hh"

/// Buffer size for reading stdout of ROS apps.
# define BUFFER_SIZE  1024

/// Minimum size to consider an array of 8bits as binary data.
# define ROS_MIN_BIN_SIZE 42


namespace ROSBinding
{
  /// Secure pointer to a ROS Type Node.
  typedef boost::shared_ptr<ROSTypeNode> ROSTypeNodePtr;

  /**
   * @brief Abstract interface for Topics and Services
   * Common tools for both subclasses.
   */
  class CommonMsg
  {
    public:
      /// Common constructor (takes a type as parameter)
      CommonMsg(const std::string& type);

      /// Returns the type provided in constructor.
      const std::string& type_get() const;

      /// Returns the cached value of the MD5.
      const std::string& md5sum_get() const;

      /**
       * @brief Describe how to serialize (headers, etc...)
       * Herited classes have to implement their own.
       */
      virtual ros::SerializedMessage serialize(
          urbi::UDictionary* d) const = 0;


      /**
       * @brief Find the size of an UList/UDictionary (for serialization).
       * \a is8bits enables the dictionary check
       * (a dictionary is a list of 8 bits data, and 8 bits only).
       * \a if expectArray is true, UBinary will be considered as a serialized list.
       */
      static uint32_t size_find(const urbi::UValue& v, bool is8bits = false, bool expectArray=false);

      /**
       * @brief Find the length of an UValue
       * @throw libport::Exception if v is not a string.
       */
      static uint32_t strlen(const urbi::UValue& v);


    protected:
      /// Check the provided type (herited classes have to reimplement).
      virtual bool type_check() = 0;

      /// Type of ros::message (provided in constructor).
      std::string type_;

      /**
       * @brief MD5 Sum of the current type required by ROS.
       * Computed by the herited class.
       */
      std::string md5sum_;


      /**
       * @brief Inner class, contaings generic-type information
       * Just a root node for the type, contains the whole type structure
       * and tools to manipulate it.
       */
      class ROSMsg
      {
        public:
          /**
           * @brief Common constructor,
           * sets the full_size_, is_fixed_size_, and has_header_.
           */
          ROSMsg();

          /// Defines a type for data to parse (stdout of rosmsg).
          typedef std::vector<std::string> V_String;

          /// Generate template structure corresponding to the current type.
          urbi::UValue build_structure() const;

          /// Generate template structure with \a node as root.
          static urbi::UValue build_structure(const ROSTypeNode* node);


          /**
           * @brief Get the size of the current node
           * The result is cached in full_size_, we need \a v to have the size
           * of dynamics length (strings and variable arrays).
           * \a v need to be a dictionary.
           */
          uint32_t full_size_calculate(const urbi::UValue& v) const;

          /**
           * @brief Get the size of the current node
           * The result is cached in full_size_, we need \a d to have the size
           * of dynamics length (strings and variable arrays).
           */
          uint32_t full_size_calculate(const urbi::UDictionary& d) const;

          /**
           * @brief Get the size of the structure pointed by \a root.
           * With \a d for variable length data.
           */
          static uint32_t full_size_get(const ROSTypeNode& root,
                                        const urbi::UDictionary& d);

          /**
           * @brief Get the size of the structure pointed by \a root.
           * With \a v for variable length data.
           * @throw libport::Exception if v is not a dictionary.
           */
          static uint32_t full_size_get(const ROSTypeNode& root,
                                        const urbi::UValue& v);


          /**
           * @brief Serialize \a d in \a buffer, according to current type.
           * Just calls serialize_tree.
           */
          void serialize(uint8_t*& buffer,
                         uint32_t& length,
                         const urbi::UDictionary& d) const;


          /**
           * @brief Serialize dictionary \a d in \a buffer
           * Use \a root as tree root node.
           */
          void serialize_tree(uint8_t*& buffer,
                              uint32_t& length,
                              const ROSTypeNode& root,
                              const urbi::UDictionary& d) const;

          /**
           * @brief Serialize dictionary in \a v in \a buffer
           * Use \a root as tree root node.
           * @throw libport::Exception is v is not a dictionary.
           */
          void serialize_tree(uint8_t*& buffer,
                              uint32_t& length,
                              const ROSTypeNode& root,
                              const urbi::UValue& v) const;

          /**
           * @brief Serialize a single value \a v in \a buffer
           * Use node \a root to know in what type we need to serialize v.
           * If \a v and \a root are incompatible, returns a default value.
           */
          void serialize_value(uint8_t*& buffer,
                               uint32_t& length,
                               const ROSTypeNode& root,
                               const urbi::UValue& v) const;

          /**
           * @brief Parse data_ and create the ROS Type Tree in node_
           * @throw libport::Exception if data_ is malformed.
           */
          void node_tree_create();


          /**
           * @brief Create a new UValue from \a buffer.
           * Calls deserialization_tree with node_
           */
          urbi::UValue* deserialize(const unsigned char* buffer,
                                    unsigned int size) const;

          /**
           * @brief Create a new UValue from \a buffer.
           * Uses \a root as root node.
           */
          urbi::UValue* deserialize_tree(const unsigned char*& buffer,
                                         unsigned int& size,
                                         const ROSTypeNode& root) const;

          /**
           * @brief Create a new UValue from the leaf \a node (with \a buffer).
           */
          urbi::UValue* deserialize_value(const unsigned char*& buffer,
                                          unsigned int& size,
                                          const ROSTypeNode& node) const;


          /// True when the message has a "Header header" as first element.
          bool has_header_;

          /// True when message is fixed size.
          bool is_fixed_size_;

          /// Calculated size of the message (cache).
          mutable uint32_t full_size_;

          /// Extended description of the type (parsed by node_tree_create).
          V_String data_;

          /// First node for the ROS Type Structure.
          ROSTypeNodePtr node_;
      };
  };
}

#include "common-msg.hxx"

#endif // ! COMMON_MSG_HH

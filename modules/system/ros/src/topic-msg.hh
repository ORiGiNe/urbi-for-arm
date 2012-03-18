/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef TOPIC_MSG_HH
# define TOPIC_MSG_HH

# include <libport/exception.hh>
# include <urbi/uobject.hh>

# include "common-msg.hh"
# include "cacheable.hh"

namespace ROSBinding
{
  /**
   * @brief Handle topic message type.
   * Used by ROSTopic to serialize, deserialize, and get info on the type.
   */
  class TopicMsg
    : public CommonMsg, public Cacheable<TopicMsg>
  {
    public:
      /// \name Ctors & Dtors
      /// \{
      /**
       * @brief Simple constructor (takes a ROS Type as parameter \a type).
       * @throw libport::Exeption if type_check returns false.
       */
      TopicMsg(const std::string& type);
      /// Copy constructor.
      TopicMsg(const TopicMsg& t);
      /// Affectation operator.
      TopicMsg& operator=(const TopicMsg& t);

      /// Nothing special.
      virtual ~TopicMsg();
      /// \}


      /// Get the full empty structure from the type we have.
      urbi::UValue structure_get() const;


      /// Serialize a dictionary \a d to ROS data.
      virtual ros::SerializedMessage serialize(
          urbi::UDictionary* d) const;

      /// Create an UValue from the raw data provided by ROS.
      urbi::UValue* deserialize(const unsigned char* buffer,
                                unsigned int size) const;

      /// Whether or not the contained type has headers.
      bool has_header() const;

      /// Whether or not the type is fixed size (no string or variable array).
      bool is_fixed_size() const;


    private:
      /**
       * @brief Checks that the provided type exists.
       * And fills an array in msg_ with the data to parse with data_parse.
       */
      virtual bool type_check();


      /**
       * @brief Generic message definition
       * With starting node of the tree and non topic-specific information.
       */
      ROSMsg msg_;
  };
}

#include "topic-msg.hxx"

#endif // ! TOPIC_MSG_HH

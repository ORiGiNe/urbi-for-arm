/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SERVICE_MSG_HH
# define SERVICE_MSG_HH

# include <libport/exception.hh>
# include <urbi/uobject.hh>

# include "common-msg.hh"
# include "cacheable.hh"

namespace ROSBinding
{
  /**
   * @brief Handle topic message type.
   * Used by ROSService to serialize, deserialize, and get info on the type.
   */
  class ServiceMsg
    : public CommonMsg, public Cacheable<ServiceMsg>
  {
    public:
      /// \name Ctors & Dtors
      /// \{
      /**
       * @brief Simple constructor (takes a ROS Type as parameter \a type).
       * @throw libport::Exeption if type_check returns false.
       */
      ServiceMsg(const std::string& type);
      /// Copy constructor.
      ServiceMsg(const ServiceMsg& t);
      /// Affectation operator.
      ServiceMsg& operator=(const ServiceMsg& t);

      /// Nothing special.
      virtual ~ServiceMsg();
      /// \}


      /// Get the full empty structure of the request.
      urbi::UValue req_struct_get() const;

      /// Get the full empty structure of the response.
      urbi::UValue res_struct_get() const;


      /// Serialize a request.
      virtual ros::SerializedMessage serialize(
          urbi::UDictionary* d) const;

      /// Create an UValue from the raw data of the response.
      urbi::UValue* deserialize(const unsigned char* buffer,
                                unsigned int size) const;


    private:
      /**
       * @brief Checks that the provided type exists.
       * And fills an array in req_ and res_ with the data to parse with
       * data_parse.
       */
      virtual bool type_check();


      /// Specific message info on the request.
      ROSMsg req_;

      /// Specific message info on the response.
      ROSMsg res_;
  };
}

#include "service-msg.hxx"

#endif // ! SERVICE_MSG_HH

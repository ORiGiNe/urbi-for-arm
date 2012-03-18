/*
 * Copyright (C) 2010-2012, Gostai S.A.S.
 *
 * This software is provided "as is" without warranty of any kind,
 * either expressed or implied, including but not limited to the
 * implied warranties of fitness for a particular purpose.
 *
 * See the LICENSE file for more information.
 */

#ifndef SERVICE_HH
# define SERVICE_HH

# include <libport/asio.hh>
# include <urbi/uobject.hh>

namespace ROSBinding
{
  typedef boost::shared_ptr<urbi::UValue const> UValueConstPtr;
  typedef boost::shared_ptr<urbi::UValue> UValuePtr;

  class ROSService;
}

# include "tools.hh"
# include "ros-overload.hh"
# include "service-msg.hh"


namespace ROSBinding
{
  /**
   * @brief UObject hander for ROS services.
   */
  class ROSService
    : public urbi::UObject
  {
    public:
      /// C++ constructor for the uobject.
      ROSService(const std::string& name);

      /// C++ destructor.
      ~ROSService();


      /**
       * @brief Urbi Constructor (takes the name of the service in \a service).
       * \a persistent determines whether or not the connection has to be kept
       * open (improve performances on multiple requests).
       * @throw libport::Exception when the service name is invalid or the
       * service is not known by roscore.
       */
      int init(const std::string& service, bool persistent);


      /**
       * @brief Request the service with the message \a req.
       */
      urbi::UDictionary request(urbi::UDictionary req);



      /**
       * @brief Contains the template structure for the service request.
       */
      urbi::UVar req_struct;

      /**
       * @brief Contains the template structure for the service response.
       */
      urbi::UVar res_struct;


      /// Get the current service name.
      const std::string& service_get() const;

      /// Whether or not the initialization is complete.
      urbi::UVar initialized_;


    private:
      /// Used internally for headers sent through the socket.
      typedef std::map<std::string, std::string> headers_type;

      /// Used internally to continue initialization after socket received.
      void init_continue(const std::string& type);

      /// Connect to the given host/port to get the message type.
      void type_get(const std::string& host, unsigned int port);

      /// Name of the current service.
      std::string service_;

      /**
        * @brief  Handle ROS service type (internal structure of message, etc...).
        *
        * Cached, shared between instances with same message type.
        */
      ServiceMsg* service_msg_;

      /// ROS Service client Object.
      ros::ServiceClient ros_client_;

      /// Whether or not the connection to the service provider is persistent.
      bool persistent_;

      /**
       * @brief Our Socket to get the type of the Service.
       */
      class ServiceTypeSocket
        : public libport::Socket
      {
      public:
        typedef libport::Socket super_type;

        /// Constructor with a link to the parent class.
        ServiceTypeSocket(ROSService* rs);

        /// Destructor.
        virtual ~ServiceTypeSocket();

        /// Overloaded method called on new data.
        virtual size_t onRead(const void*, size_t length);

        /// Also set rs_->socket_ to 0.
        virtual void doDestroy();

      protected:
        /// Pointer to the caller.
        ROSService* rs_;
      };


      /// Needed by ServiceTypeSocket to call init_continue.
      friend size_t ServiceTypeSocket::onRead(const void*, size_t);

      /// Used internally to get the service type.
      ServiceTypeSocket *socket_;
  };
}

# include "service.hxx"

#endif // ! SERVICE_HH

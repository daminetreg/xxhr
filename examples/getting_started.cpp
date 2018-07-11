#include <iostream>

//! [include-header]
#include <xxhr/xxhr.hpp>
//! [include-header]

int main() {

//! [using-xxhr]
  using namespace xxhr;
//! [using-xxhr]

//! [GET-query]
  GET( "https://tools.ietf.org/rfc/rfc2616"s, 
    on_response = [](auto&& resp) {

      std::cout << resp.text;

    }
  );
//! [GET-query]

//! [GET-query-error-handling]
  GET( "https://tools.ietf.org/rfc/rfc2616"s, 
    on_response = [](auto&& resp) {
      
      if (!resp.error) {
        
        std::cout << resp.text;
      } else {
        
        std::cout << resp.error;
      }
    }
  );
//! [GET-query-error-handling]

//TODO: can I remove Url ? in favor of string (at least with suffix)
  
//! [GET-query-timeout]

  auto retries_count = 3;
  std::function<void(xxhr::Response&&)> retry_on_fail;

  auto do_request = [&]() { 
    GET( 
      "https://tools.ietf.org/rfc/rfc2616"s,
      on_response = retry_on_fail,
      Timeout(3s)
    );
  };

  retry_on_fail = [&](auto&& resp) {

    if (!resp.error) {

      std::cout << resp.text;

    } else {

      if (resp.error.code == +ErrorCode::TIMEDOUT) {

        if (retries_count > 0) {
          std::cout << "Timed Out, retrying : " << std::endl;
          --retries_count;
          do_request();
        } else {
          std::cout << "Timed Out aborting." << std::endl;
        }
      } else {
        std::cout << "Please plug back the Internet : " << resp.error;
      }
    }
  };

  do_request();

//! [GET-query-timeout]

  return 0;
}


/*! \page getting-started-cpp Getting Started
  \tableofcontents

  ## Include xxhr convenience header
  The library is really lightweight, therefore it makes sense to include all definitions at once.
  
  \snippet this include-header

  ## Supposed using declaration
  All documentation in this library suppose the use at function scope of :
  \snippet this using-xxhr

  ## Select the type of HTTP Request
  In a [RESTful](https://en.wikipedia.org/wiki/Representational_state_transfer) model the CRUD operations are the entrypoints of Requests, and so is the entrypoint of xxhr : 

  - [GET](@ref xxhr::GET)
  - [POST](@ref xxhr::POST) 
  - [PUT](@ref xxhr::PUT) 
  - [HEAD](@ref xxhr::HEAD) 
  - [DELETE](@ref xxhr::DELETE) 
  - [OPTIONS](@ref xxhr::OPTIONS) 
  - [PATCH](@ref xxhr::PATCH) 
   
  From there on, all goes asynchronous with the [on_response](@ref xxhr::on_response) continuation handler which expects a lambda or whatever Callable which accepts a [Response](@ref xxhr::Response) : 

  \snippet this GET-query

  ## Under HTTP it can go wrong
  HTTP is the icing on the cake of network communications, there are alot of OSI layer below, that we don't care of but which might prevent your app from running correctly.

  Therefore you can check an xxhr::Response for the `resp.error` field :
  \snippet this GET-query-error-handling 
  
  ### users aren't patient
  To improve your application resiliency you can specify an xxhr::Timeout with the query, which will calls the on_response callback early with the xxhr::Error code : `ErrorCode::TIMEDOUT`.

  \snippet this GET-query-timeout
*/

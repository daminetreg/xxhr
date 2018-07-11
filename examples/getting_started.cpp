#include <iostream>

//! [include-header]
#include <xxhr/xxhr.hpp>
//! [include-header]

int main() {

//! [using-xxhr]
  using namespace xxhr;
//! [using-xxhr]

//! [GET-request]
  GET( "https://tools.ietf.org/rfc/rfc2616"s, 
    on_response = [](auto&& resp) {

      std::cout << resp.text;

    }
  );
//! [GET-request]

//! [GET-request-error-handling]
  GET( "https://tools.ietf.org/rfc/rfc2616"s, 
    on_response = [](auto&& resp) {
      
      if (!resp.error) {
        
        std::cout << resp.text;
      } else {
        
        std::cout << resp.error;
      }
    }
  );
//! [GET-request-error-handling]

//TODO: can I remove Url ? in favor of string (at least with suffix)
  
//! [GET-request-timeout]

  GET( 
    "https://tools.ietf.org/rfc/rfc2616"s,
    Timeout{3s},
    on_response = [](auto&& resp) {
      if (resp.error.code == +ErrorCode::TIMEDOUT)
        std::cout << "Timed out !\n";
    }
  );

//! [GET-request-timeout]

//! [GET-request-retry]

  auto retries_count = 3;
  std::function<void(xxhr::Response&&)> retry_on_fail;

  auto do_request = [&]() { 
    GET( 
      "https://tools.ietf.org/rfc/rfc2616"s,
      on_response = retry_on_fail
    );
  };

  retry_on_fail = [&](auto&& resp) {
    

    if ( resp.error && (retries_count > 0) ) {
      std::cout << "Error : " << resp.error << " retrying \n";
      --retries_count;
      do_request();
    } else {
      // do something with the last resp
    }
  };

  do_request();

//! [GET-request-retry]

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
   
  From there on, all goes asynchronous with the [on_response](@ref xxhr::on_response) continuation handler which expects a lambda or whatever Callable which accepts a [Response](@ref xxhr::Response).

  To get started, nothing better than downloading the everlasting HTTP Request For Comment : 

  \snippet this GET-request

  ## Below HTTP it can go wrong
  HTTP is the icing on the cake of network communications, there are alot of OSI layer below, that we don't care of but which might prevent your app from running correctly.

  Therefore you can check an xxhr::Response for the `resp.error` field :
  \snippet this GET-request-error-handling 
  
  ### users aren't patient
  To avoid blocking your application with never ending HTTP requests you can specify an xxhr::Timeout, which will calls the on_response callback early with the xxhr::Error code : `ErrorCode::TIMEDOUT`.

  \snippet this GET-request-timeout

  ### Try again !
  It's always good to give some more chances to the same action, this can easily be achieved by wrapping the request calls in a lambda :

  \snippet this GET-request-retry
*/

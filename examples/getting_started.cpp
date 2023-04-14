#include <iostream>

//! [include-header]
#include <xxhr/xxhr.hpp>
//! [include-header]

int main() {

//! [using-xxhr]
  using namespace xxhr;
//! [using-xxhr]

//! [GET-request]
  GET( "http://httpbin.org/anything"s, 
    on_response = [](auto&& resp) {

      std::cout << resp.text;

    }
  );
//! [GET-request]

//! [GET-request-error-handling]
  GET( "http://httpbin.org/anything"s, 
    on_response = [](auto&& resp) {
      
      if (!resp.error) {
        
        std::cout << resp.text;
      } else {
        
        std::cout << resp.error;
      }
    }
  );
//! [GET-request-error-handling]

//! [GET-request-timeout]

  GET( 
    "http://httpbin.org/anything"s,
    Timeout{3s},
    on_response = [](auto&& resp) {
      if (resp.error.code == +ErrorCode::TIMEDOUT)
        std::cout << "Timed out !\n";
    }
  );

//! [GET-request-timeout]


//! [GET-request-download-to]
  auto downloadFinished = [](const xxhr::Response &resp, const std::string &output_path, const size_t file_size) {

    if(resp.status_code != 200) {
      std::cout << "Oh noes - got HTTP " << resp.status_code << " / error: " << resp.error << "!\n";
    }
    else {
      std::cout << "Downloaded " << file_size << "bytes to " << output_path << "!\n"; 
    }
    
  };

  auto progressMeter = [](size_t dltotal, size_t dlnow) {
    std::cout << "[PROGRESS] " << dlnow << " of " << dltotal << "\n";
  };

  GET( "https://nxxm.indigenious.io/distro/all/clang/v13.0.0/clang-13-windows-64bit.zip"s, 
    //Header{ { "Accept", "image/jpeg" }},  
    DownloadTo("test.out.zip", downloadFinished, progressMeter)
  );
//! [GET-request-download-to]


  return 0;
}


/*! \page getting-started-cpp GETting Started
  \tableofcontents

  ## Download 
  Drop the following in your [`.nxxm/deps`](https://nxxm.github.io) : 

  ```
  {
    "daminetreg/xxhr" : { "@" : "v1.0.0" }
  }
  ```

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
  - [DELETE](@ref xxhr::DELETE_) 
  - [OPTIONS](@ref xxhr::OPTIONS) 
  - [PATCH](@ref xxhr::PATCH) 
   
  From there on, all goes asynchronous with the [on_response](@ref xxhr::on_response) continuation handler which expects a lambda or whatever Callable which accepts a [Response](@ref xxhr::Response).

  To get started, nothing better than downloading the everlasting HTTP Request For Comment : 

  \snippet this GET-request

  ## Below HTTP it can go wrong
  HTTP is the icing on the cake of network communications, there are alot of OSI layer below, that we shouldn't care of but which might prevent your app from running correctly.

  Therefore you can check an xxhr::Response for the `resp.error` field :
  \snippet this GET-request-error-handling 
  
  ### users aren't patient
  To avoid blocking your application with never ending HTTP requests you can specify an xxhr::Timeout, which will calls the on_response callback early with the xxhr::Error code : `ErrorCode::TIMEDOUT`.

  \snippet this GET-request-timeout

  For more details see @ref retrying-cpp 

*/

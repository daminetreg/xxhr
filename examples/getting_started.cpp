#include <iostream>

//! [include-header]
#include <xxhr/xxhr.hpp>
//! [include-header]

int main() {

//! [using-xxhr]
  using namespace xxhr;
//! [using-xxhr]

//! [GET-query]
  GET( Url{"http://www.lecbna.org/index.php"}, 
    on_response = [](auto&& resp) {
      if (resp.status_code == 200) {
        std::cout << resp.text; 
      } else {
        std::cout << "Status : " << resp.status_code 
          << resp.error << std::endl;
      }
    },
    Timeout(3000ms)
  );
//! [GET-query]

  return 0;
}


/*! \page getting-started-cpp Getting Started
  \tableofcontents

  ## Include xxhr convenience header
  The library is really lightweight, therefore it makes sense to include all definitions at once.
  
  \snippet this include-header

  ## Suposed using declaration
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
   
  From there on, all goes asynchronous with the [on_response](@ref xxhr::on_response) named variable which expects a lambda or whatever Callable which accepts an [xxhr::response](@ref xxhr::response] : 

  \snippet this GET-query

*/

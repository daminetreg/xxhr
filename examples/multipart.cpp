#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

//! [Multipart-snippet]
  POST("http://httpbin.org/post"s, 
    
    Multipart{
      {"somefile", File{"../../xxhr/api.hpp"} }
    },

    on_response = [](auto&& resp) {
      std::cout << resp.text;
      std::cout << "Upload status : " << resp.error << " - " << resp.status_code << "\n";
    });
//! [Multipart-snippet]


//! [Multipart-snippet-many]
  auto buffer = "this is some buffer"s;

  POST("http://httpbin.org/post"s, 
    
    Multipart{
      {"somefile", File{"../../xxhr/api.hpp"} }
      , {"other", File{"../../xxhr/multipart.hpp"} }
      , {"and_another", File{"../../README.md"} }
      , {"somebuffer", Buffer{buffer.begin(), buffer.end(), "IN_MEMORY_FILE"} }
    },

//! [Multipart-snippet-many]

    on_response = [](auto&& resp) {
      std::cout << resp.text;
      std::cout << "Upload status : " << resp.error << " - " << resp.status_code << "\n";
    });

  return 0;
}

/*! \page multipart-cpp Uploading files
  
  Uploading files requires the use Multipart, with xxhr this is really simple.

  Given a web server expecting a field named **somefile** an xxhr::File or xxhr::Buffer can be passed as in the following example.

  \snippet examples/multipart.cpp Multipart-snippet

  ## Multiple files at once
  It is possible, as it is a Multipart query to add many xxhr::File or xxhr::Buffer.

  \snippet examples/multipart.cpp Multipart-snippet-many

*/


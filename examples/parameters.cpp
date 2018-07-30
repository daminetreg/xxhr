#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

//! [Parameters-snippet]
  GET("https://www.google.com/search"s, 

    Parameters{
      {"q", "xxhr c++ library"},
      {"ie", "utf-8"},
    }, 

    on_response = [](auto&& resp) {
      std::cout << "What Google finds about this library : " << resp.text;
    });
//! [Parameters-snippet]







//! [Parameters-snippet-POST]
  POST("http://httpbin.org/post"s, 

    // This will end up in the URI as : http://httpbin.org/post?id=43&lang=en
    Parameters{
      {"id", "43"},
      {"lang", "en"},
    },

    // While these params will be form-encoded parameters in the body
    Body{
      Parameters{
        {"parameter_name_in_body", "xxhr c++ library"},
        {"some_other_data", "As well in the HTTP body"},
      }
    }, 

    on_response = [](auto&& resp) {
      std::cout << "Reply from httpbin to see passed parameters : " << resp.text;
    });
//! [Parameters-snippet-POST]







  POST("http://httpbin.org/post"s, 

    // This will end up in the URI as : http://httpbin.org/post?id=43&lang=en
    Parameters{
      {"id", "43"},
      {"lang", "en"},
    },

//! [Parameters-snippet-POST-raw]
    // Here the body of the HTTP request is send as passed
    Body{
      "Big payload for some POST requests transmitted untouched"
    }, 
//! [Parameters-snippet-POST-raw]

    on_response = [](auto&& resp) {
      std::cout << "Reply from httpbin to see passed parameters : " << resp.text;
    });



  return 0;
}

/*! \page parameters-cpp Passing POST, PUT or GET parameters
 *
 *  xxhr::Parameters allows to provide a list of parameters that will be passed to the corresponding url.
 *
 *  \tableofcontents
 *  @tableofcontents 
 *
 *  ## GET parameters
 *  Those parameters are appended URI Encoded to the URI, therefore note that sensitive information might be leaked to HTTP Proxy logs or other infrastructure in between xxhr and the server.
 *
 *  Let's query google search : 
 *
 *  \snippet this Parameters-snippet
 *
 *  ## POST form urlencoded Parameters
 *  While you can perform an xxhr::Multipart POST request, there is often for non-file upload forms the use of POST requests with `application/x-www-form-urlencoded` parameters.
 *
 *  In this context, you might also need to send URI xxhr::Parameters as in the example above.
 *
 *  Therefore sending form-encoded parameter can be combined as follow : 
 *
 *  \snippet this Parameters-snippet-POST
 *
 *  ## POST Raw payload
 *  One can send a raw POST Payload without any encoding by instead passing a string for the xxhr::Body parameter : 
 *  
 *  \snippet this Parameters-snippet-POST-raw
 *
 *  ## Works for any HTTP verbs
 *  Naturally all the above works for any HTTP verbs: POST, PUT, OPTIONS...

*/

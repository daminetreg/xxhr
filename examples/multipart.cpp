#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

//! [Multipart-snippet]
  POST("http://ptsv2.com/t/xxhr/post"s, 
    
    Multipart{
      {"somefile", File{"../xxhr/api.hpp"} }
    },

    on_response = [](auto&& resp) {
      std::cout << resp.text;
      std::cout << "Upload status : " << resp.error << " - " << resp.status_code << "\n";
    });
//! [Multipart-snippet]

  return 0;
}

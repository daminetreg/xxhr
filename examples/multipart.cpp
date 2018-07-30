#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

//! [Multipart-snippet]
  POST("http://127.0.0.1/multipart/upload.php"s, 
    
    Multipart{
      //{"somefile", File{"../xxhr/api.hpp"} }
      {"somefile", File{"/home/daminetreg/Desktop/Yabe.desktop"} }
    },

    on_response = [](auto&& resp) {
      std::cout << resp.text;
      std::cout << "Upload status : " << resp.error << " - " << resp.status_code << "\n";
    });
//! [Multipart-snippet]

  return 0;
}

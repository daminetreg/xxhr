#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

  bool successful = false;
  std::string content_there;
  
  // Github CDN
  GET("https://codeload.github.com/hunter-packages/gate/zip/42c1c27cdd556dcbc358c6ba8ad7a72a68ee2a62"s, 
    on_response = [&](auto&& resp) {
      std::cout << "At target :  " << resp.text.size() << std::endl;
      content_there = resp.text;
    }
  );
    
  // Github redirects will to it's CDN
  GET("https://github.com/hunter-packages/gate/archive/42c1c27cdd556dcbc358c6ba8ad7a72a68ee2a62.zip"s, 
    on_response = [&](auto&& resp) {
      std::cout << "Success, followed the redirection : " << resp.text.size() << std::endl;
      successful = (content_there == resp.text);
    }
  );
    
  return (successful ? 0 : 1);
}

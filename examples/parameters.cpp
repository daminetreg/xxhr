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

  return 0;
}

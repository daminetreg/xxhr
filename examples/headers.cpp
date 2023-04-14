#include <iostream>

#include <xxhr/xxhr.hpp>

int main() {

  using namespace xxhr;
  GET("http://httpbin.org/anything"s,

    Header{
      {"refresh", "yep"},
      {"Banana", "23"},
      {"Content-Type", "application/octet-stream"}
    },

    on_response = [](auto&& resp) {
      std::cout << resp.text;
  });

  return 0;
}
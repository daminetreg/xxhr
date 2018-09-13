#include <iostream>

#include <xxhr/xxhr.hpp>

void introductory() {

  using namespace xxhr;

  GET( "http://httpbin.org/anything"s, 
    on_response = [](auto reply) { std::cout << reply.text; }
  );

}

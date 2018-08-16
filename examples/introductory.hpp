#include <iostream>

#include <xxhr/xxhr.hpp>

void introductory() {

  using namespace xxhr;

  GET( "https://tools.ietf.org/rfc/rfc2616"s, 
    on_response = [](auto reply) { std::cout << reply.text; }
  );

}

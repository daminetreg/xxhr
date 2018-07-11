#include <iostream>

#include <xxhr/xxhr.hpp>

void introductory() {

  using namespace xxhr;

  GET( "http://www.lecbna.org/"s, 
    on_response = [](auto&& resp) {
      std::cout << resp.text;
    }
  );

}

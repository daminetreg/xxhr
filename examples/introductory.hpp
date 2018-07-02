#include <iostream>

#include <xxhr/xxhr.hpp>

void introductory() {

  using namespace xxhr;

  GET( Url{"http://www.lecbna.org/"}, 
    on_response = [](auto&& resp) {
      std::cout << resp.text;
    }
  );

}

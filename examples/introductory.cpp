#include <iostream>

#include <xxhr/xxhr.hpp>

int main() {

  using namespace xxhr;

  GET( Url{"http://www.lecbna.org/"}, 
    on_response = [](auto&& resp) {
      if (resp.status_code == 200) { std::cout << resp.text; }
    }
  );

  return 0;
}

#include <iostream>

#include <xxhr/xxhr.hpp>

int main() {

  using namespace xxhr;

  GET( Url{"http://www.lecbna.org/index.php"}, 
    on_response = [](auto&& resp) {
      if (resp.status_code == 200) {
        std::cout << resp.text; 
      } else {
        std::cout << "Status : " << resp.status_code 
          << resp.error << std::endl;
      }
    },
    Timeout(4)
  );

  return 0;
}

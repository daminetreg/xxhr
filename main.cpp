#include <xxhr/xxhr.hpp>
#include <iostream>

int main(int argc, char** argv) {



    xxhr::GET(xxhr::Url{"CMakeCache.txt"},
                xxhr::Authentication{"user", "pass"},
                xxhr::Parameters{{"anon", "true"}, {"key", "value"}},
                xxhr::on_success = [](auto&& resp){
                  std::cout << "So we got successful ! " << std::endl;
                  std::cout << "resp.status_code " << resp.status_code << std::endl;
                  std::cout << "resp.text (40 first chars) " << resp.text.substr(0, 40) << std::endl;
                }
            );



    xxhr::GET(xxhr::Url{"ERROR IS GUARANTEED.game"},
                      xxhr::Authentication{"user", "pass"},
                      xxhr::Parameters{{"anon", "true"}, {"key", "value"}});
}

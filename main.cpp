#include <xxhr/xxhr.hpp>
#include <iostream>

int main(int argc, char** argv) {



    xxhr::GET(xxhr::Url{"CMakeCache.txt"},
                xxhr::Authentication{"user", "pass"},
                xxhr::Parameters{{"anon", "true"}, {"key", "value"}},
                xxhr::Cookies{{"hallo", "hello"}},
                xxhr::on_response = [](auto&& resp){
                  std::cout << "So we got successful ! " << std::endl;
                  std::cout << "resp.status_code " << resp.status_code << std::endl;
                  std::cout << "resp.text (40 first chars) " << resp.text.substr(0, 40) << std::endl;
                  std::cout << "there are " << resp.header.size() << " headers " << std::endl;
                  for (auto& header : resp.header) {
                    std::cout << "\t" << header.first << " ---- " << header.second << std::endl;
                  }
                }
            );



    xxhr::GET(xxhr::Url{"http://bonjour-daminou.com/ERROR IS GUARANTEED.game"},
                    xxhr::Authentication{"user", "pass"},
                    xxhr::Parameters{{"anon", "true"}, {"key", "value"}},
                    xxhr::on_response = [](auto&& resp){

                      if (resp.error) {
                        std::cout << "Connection Error !" << std::endl;
                      }
                    });
}

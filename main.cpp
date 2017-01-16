#include <xxhr/xxhr.hpp>
#include <iostream>

int main(int argc, char** argv) {
    auto r = xxhr::GET(xxhr::Url{"CMakeCache.txt"},
                      xxhr::Authentication{"user", "pass"},
                      xxhr::Parameters{{"anon", "true"}, {"key", "value"}});
    std::cout << r.status_code << std::endl;                  // 200
    std::cout << r.header["content-type"] << std::endl;       // application/json; charset=utf-8
    std::cout << r.text << std::endl;                         // JSON text string
}

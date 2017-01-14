#include <xxhr/xxhr.hpp>

int main(int argc, char** argv) {
    auto r = xxhr::GET(xxhr::Url{"https://api.github.com/repos/whoshuu/xxhr/contributors"},
                      xxhr::Authentication{"user", "pass"},
                      xxhr::Parameters{{"anon", "true"}, {"key", "value"}});
    r.status_code;                  // 200
    r.header["content-type"];       // application/json; charset=utf-8
    r.text;                         // JSON text string
}

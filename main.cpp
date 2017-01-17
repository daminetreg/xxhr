#include <xxhr/xxhr.hpp>
#include <iostream>

int main(int argc, char** argv) {
    auto r = xxhr::GET(xxhr::Url{"CMakeCache.txt"},
                      xxhr::Authentication{"user", "pass"},
                      xxhr::Parameters{{"anon", "true"}, {"key", "value"}});

    auto s = xxhr::GET(xxhr::Url{"ERROR IS GUARANTEED.game"},
                      xxhr::Authentication{"user", "pass"},
                      xxhr::Parameters{{"anon", "true"}, {"key", "value"}});
}

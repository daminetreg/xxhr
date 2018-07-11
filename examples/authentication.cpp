#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

//! [Authentication-snippet]
  GET("https://api.github.com/search/code"s,

    Authentication{"daminetreg", "your-github-security-token"},

    Parameters{{"q", "authenticated search string"}}, 
    on_response = [](auto&& resp) {
      std::cout << resp.text;
    });
//! [Authentication-snippet]

  return 0;
}

#include <iostream>

#include <xxhr/xxhr.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;

//! [Authentication-snippet]
  // For example connecting to the Github API v3 with your Github Credentials
  GET("https://api.github.com/search/code"s,

    Authentication{"username", "password"},
    // Will be transmitted in cleartext base64 encoded, therefore the use of https is recommended.

    Parameters{{"q", "authenticated search string"}}, 
    on_response = [](auto&& resp) {
      std::cout << resp.text;
    });
//! [Authentication-snippet]

//! [Authentication-snippet-digest]
  // For example connecting to the Github API v3 with your Github Credentials
  GET("https://api.github.com/search/code"s,

    Digest{"username", "password"},
    // Will be transmitted as MD5 hash.

    Parameters{{"q", "authenticated search string"}}, 
    on_response = [](auto&& resp) {
      std::cout << resp.text;
    });
//! [Authentication-snippet-digest]

  return 0;
}

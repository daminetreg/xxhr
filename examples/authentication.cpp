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

//! [Authentication-snippet-bearer]
  GET("http://httpbin.org/bearer"s,

    Bearer{"I dont have a token but I can put it on my karma"},

    on_response = [](auto&& resp) {
      std::cout << resp.text;
    });
//! [Authentication-snippet-bearer]



  return 0;
}

/*! \page authentication-cpp HTTP Authentication
  \tableofcontents

  Passing an xxhr::Authentication is enough to transmit user and password information. In HTTP there are two ways : Basic and Digest authentication.

  It is recommended to use authentication, even the Digest one only on a TLS connection.

  ## Basic Authentication
  In this example we query the Github REST search api, if you've setup 2-Factor Authentication on your github account you should use a [github personal access token](https://help.github.com/articles/creating-a-personal-access-token-for-the-command-line/) to test this example and use it in place of password.

  Otherwise you'll get an xxhr::Response::status_code 404.

  \snippet examples/authentication.cpp Authentication-snippet

  ## Digest Authentication

  To initiate a Digest authentication use in-place of xxhr::Authentication the xxhr::Digest parameter: 

  \snippet examples/authentication.cpp Authentication-snippet-digest

  ## Bearer Authentication

  To initiate a Bearer authentication use xxhr::Bearer parameter: 

  \snippet examples/authentication.cpp Authentication-snippet-bearer

*/

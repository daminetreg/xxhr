#include <xxhr/xxhr.hpp>
#include <boost/process/environment.hpp>

#include <iostream>

int main(int argc, char** argv) {

  using namespace xxhr;

  // In code
  GET( "https://httpbin.org/anything"s, 
    Proxies{ 
      {"http", "http://36.66.124.193:3128"}
      , {"https", "http://36.66.124.193:3128"}
    },  // Proxy from https://free-proxy-list.net
    on_response = [](auto reply) { 
      if (!reply.error) {
        std::cout << reply.text << "\n";
      } else {
        std::cout << reply.error << "\n";
      }
    }
  );


  // With an environment variable
  auto env = boost::this_process::environment();
  env["ALL_PROXY"] = "http://75.119.146.236:6969"; // Proxy from https://free-proxy-list.net/

  GET( "http://httpbin.org/anything"s, 
    on_response = [](auto reply) { 
      if (!reply.error) {
        std::cout << reply.text;
      } else {
        std::cout << reply.error;
      }
    }
  );

  return 0;
}

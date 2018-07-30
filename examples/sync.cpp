#include <xxhr/xxhr.hpp>
#include <iostream>

#include <xxhr/sync.hpp>


int main(int argc, char** argv) {

  using namespace xxhr;
  //! [sync-snippet]
  xxhr::sync sync_;
  GET("https://tools.ietf.org/rfc/rfc2616"s, on_response = sync_);
  
  // blocks until response arrives
  auto response = sync_();

  std::cout << "Finally we got the reply : " << response.status_code << " and content : " << response.text << std::endl;
  //! [sync-snippet]
}

/*! \page syncapi-cpp Synchronous Helper
  
  In xxhr there is nothing like a Synchronous API. While we think it's a bad idea to write anything synchronously, there are cases where it makes sense and it might be needed.

  The issue with code which waits on IO is that they are non-welcome in the Web Browser, in new Windows apps or mobile platforms as they block the user from performing anything else until the operation complete.
  
  Network operations are full of latency, therefore it is a pretty bad idea to read this example and to use it in your code. Because basically you can block your app forever (you might use an xxhr::Timeout to mitigate it, but the user isn't as patient as network operations are slow).

  But if really it's your last resort, then you can simply use the xxhr::sync helper as follow :  

  \snippet examples/sync.cpp sync-snippet

*/

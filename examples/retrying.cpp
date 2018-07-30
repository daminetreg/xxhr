#include <xxhr/xxhr.hpp>

int main() {

  using namespace xxhr;

//! [GET-request-retry-vars]

  auto retries_count = 3;
  std::function<void(xxhr::Response&&)> retry_on_fail;

//! [GET-request-retry-vars]


//! [GET-request-retry]
  auto do_request = [&]() { 
    GET( 
      "https://tools.ietf.org/rfc/rfc2616"s,
      on_response = retry_on_fail
    );
  };
//! [GET-request-retry]


//! [GET-request-retry-onfail]
  retry_on_fail = [&](auto&& resp) {
    

    if ( resp.error && (retries_count > 0) ) {
      --retries_count;
      do_request();
    } else {
      // correct response
    }
  };
//! [GET-request-retry-onfail]

//! [GET-request-retry-do]
  do_request();
//! [GET-request-retry-do]

  return 0;
}

/*! \page retrying-cpp Retry on failure 

  It's always good to give some more chances to the same action, this can easily be achieved by wrapping the request calls :

  ## on_response handler

  We can use the indirection offered by `std::function`, so that the request and the on_response handler can respectively refers each other.

  \snippet this GET-request-retry-vars

  ## define the request 

  \snippet this GET-request-retry

  ## on_response definition
  Now we can write an on_response handler which will calls the request again if it failed and there are still `retries_count` available.

  \snippet this GET-request-retry-onfail

  ## Don't forget to initiate the first try 

  \snippet this GET-request-retry-do

*/

#ifndef XXHR_SYNC_HPP
#define XXHR_SYNC_HPP

#include <xxhr/response.hpp>
#include <future>

namespace xxhr {

  /**
   * \brief Helper object for the non-recommended good old synchronous
   *        way of thinking.
   *
   * **Note** that using this will not have great performance when you
   * compile for the web browser platform. This will hang the browser UI
   * until the request complete and some browsers (chrome) might not
   * allocate execution time for the request to happen.
   *
   * \copydoc syncapi-cpp
   *
   */
  struct sync {

    std::shared_ptr<std::promise<xxhr::Response>> sync 
      = std::make_shared<std::promise<xxhr::Response>>();

    void operator()(xxhr::Response&& resp) { sync->set_value(resp); };

    xxhr::Response operator()() { return sync->get_future().get(); };
  };
}

#endif

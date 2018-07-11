#ifndef XXHR_APISYNC_HPP
#define XXHR_APISYNC_HPP

#include <xxhr/response.hpp>
#include <future>

namespace xxhr {

  /**
   * \brief Helper object for the non-recommended good old synchronous
   *        way of programming.
   *
   * **Note** that using this will not have great performance when you
   * compile for the web browser platform. This will hang the browser UI
   * until the request complete and some browsers (chrome) might not
   * allocate execution time for the request to happen.
   *
   * ```cpp
   * xxhr::sync sync_;
   * xxhr::GET(xxhr::Url{"http://somewhere.com"}, 
   *   xxhr::on_response = sync_);
   * auto response = sync_();
   * ```
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

#ifndef XXHR_HANDLER_HPP
#define XXHR_HANDLER_HPP

namespace xxhr {

  template<template <class> class Template>
  struct make_handler_t {
      template<typename Handler>
      constexpr Template<Handler> operator()(Handler handler) const {return Template<Handler>(handler);}
      template<typename Handler>
      constexpr Template<Handler> operator= (Handler handler) const {return Template<Handler>(handler);}
      template<typename Handler>
      constexpr Template<Handler> operator+=(Handler handler) const {return Template<Handler>(handler);}
  };

  struct handler {};

  template <class Handler>
  struct on_response_ : handler
  {
    explicit on_response_(Handler handler) : handler_(handler) {}

    template<class TResponse>
    void operator()(TResponse&& e)
    {
        handler_(XXHR_FWD(e));
    }

    private:
    Handler handler_;
  };

  constexpr make_handler_t<on_response_> on_response;
}

#endif // XXHR_HANDLER_HPP

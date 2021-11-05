#ifndef XXHR_DETAIL_SESSION_HPP
#define XXHR_DETAIL_SESSION_HPP

#if defined(__EMSCRIPTEN__)
  #include <xxhr/impl/session-emscripten.hpp> // Browser javascript implementation
#elif defined(XXHR_BEAST)
  #include <xxhr/impl/session-beast.hpp> // boost asio beast implementation
#else 
  #include <xxhr/impl/session-curl.hpp> // curl implementation
#endif

#endif

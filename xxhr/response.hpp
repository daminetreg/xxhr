#ifndef XXHR_RESPONSE_H
#define XXHR_RESPONSE_H

#include <cstdint>
#include <string>

#include "cookies.hpp"
#include "xxhrtypes.hpp"
#include "defines.hpp"
#include "error.hpp"

namespace xxhr {
class Response {
  public:
    Response() = default;

    template <typename TextType, typename HeaderType, typename UrlType, typename CookiesType,
              typename ErrorType>
    Response(const std::int32_t& p_status_code, TextType&& p_text, HeaderType&& p_header, UrlType&& p_url, 
        CookiesType&& p_cookies = Cookies{},
             ErrorType&& p_error = Error{})
            : status_code{p_status_code}, text{XXHR_FWD(p_text)}, header{XXHR_FWD(p_header)},
              url{XXHR_FWD(p_url)}, cookies{XXHR_FWD(p_cookies)},
              error{XXHR_FWD(p_error)} {}

    std::int32_t status_code;
    std::string text;
    Header header;
    Url url;
    Cookies cookies;
    Error error;
};

} // namespace xxhr

#endif

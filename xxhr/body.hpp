#ifndef XXHR_BODY_H
#define XXHR_BODY_H

#include <cstring>

#include <initializer_list>
#include <string>

#include "defines.hpp"

#include <xxhr/parameters.hpp>

namespace xxhr {

/**
 * \brief The body sent with the request. Typically useful in xxhr::POST context but can be used in any request type.
 */
struct Body {
  Body() = default;
  Body(const Body& rhs) = default;
  Body(Body&& rhs) = default;
  Body& operator=(const Body& rhs) = default;
  Body& operator=(Body&& rhs) = default;

  Body(const std::string& std_string) : content(std_string), is_form_encoded(false) {}
  Body(const Parameters& params) : content(params.content), is_form_encoded(true) {}

  std::string content;
  bool is_form_encoded;
};

} // namespace xxhr

#endif

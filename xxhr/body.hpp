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
 * 
 * With these you either construct a raw body from an `std::string` or a form-urlencoded body from an xxhr::Parameters.
 *
 * ## Examples
 * @ref parameters-cpp 
 *
 */
struct Body {
  Body() = default;
  Body(const Body& rhs) = default;
  Body(Body&& rhs) = default;
  Body& operator=(const Body& rhs) = default;
  Body& operator=(Body&& rhs) = default;

  //! Construct a raw body from a string
  Body(const std::string& std_string) : content(std_string), is_form_encoded(false) {}

  //! Construct an `application/x-www-form-urlencoded` Body from an xxhr::Parameter list
  Body(const Parameters& params) : content(params.content), is_form_encoded(true) {}

  std::string content;
  bool is_form_encoded;
};

} // namespace xxhr

#endif

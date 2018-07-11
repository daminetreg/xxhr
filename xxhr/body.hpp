#ifndef XXHR_BODY_H
#define XXHR_BODY_H

#include <cstring>

#include <initializer_list>
#include <string>

#include "defines.hpp"

namespace xxhr {

/**
 * \brief The body sent with the request. Typically useful in xxhr::POST context but can be used in any request type.
 */
class Body : public std::string {
  public:
    Body() = default;
    Body(const Body& rhs) = default;
    Body(Body&& rhs) = default;
    Body& operator=(const Body& rhs) = default;
    Body& operator=(Body&& rhs) = default;

    //!
    explicit Body(const char* raw_string) : std::string(raw_string) {}
    //!
    explicit Body(const char* raw_string, size_t length) : std::string(raw_string, length) {}
    //!
    explicit Body(size_t to_fill, char character) : std::string(to_fill, character) {}
    //!
    explicit Body(const std::string& std_string) : std::string(std_string) {}
    //!
    explicit Body(const std::string& std_string, size_t position, size_t length = std::string::npos)
            : std::string(std_string, position, length) {}
    //!
    explicit Body(std::initializer_list<char> il) : std::string(il) {}
    //!
    template <class InputIterator>
    explicit Body(InputIterator first, InputIterator last)
            : std::string(first, last) {}
};

} // namespace xxhr

#endif

#ifndef XXHR_TIMEOUT_H
#define XXHR_TIMEOUT_H

#include <chrono>
#include <cstdint>

#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace xxhr {
  using namespace std::literals::chrono_literals; // Improves readability of user code by allowing to write suffixes.

/**
 * \brief 
 */
class Timeout {
  public:
    Timeout(const std::chrono::milliseconds& duration) : ms{duration} {}
    Timeout(const std::int32_t& milliseconds) : Timeout{std::chrono::milliseconds(milliseconds)} {}
    template< class Rep, class Period > 
    Timeout(const std::chrono::duration<Rep, Period> duration) 
      : Timeout(std::chrono::duration_cast<std::chrono::milliseconds>(duration)) {}

    std::chrono::milliseconds ms;
};

} // namespace xxhr

#endif

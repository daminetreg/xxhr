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

class Timeout {
  public:
    Timeout(const std::chrono::milliseconds& duration) : ms{duration} {}
    Timeout(const std::int32_t& milliseconds) : Timeout{std::chrono::milliseconds(milliseconds)} {}

    long Milliseconds() const {
      static_assert(std::is_same<std::chrono::milliseconds, decltype(ms)>::value,
                    "Following casting expects milliseconds.");

      if (ms.count() > std::numeric_limits<long>::max()) {
        throw std::overflow_error("cpr::Timeout: timeout value overflow: " +
                                    std::to_string(ms.count()) + " ms.");
      }
      if (ms.count() < std::numeric_limits<long>::min()) {
        throw std::underflow_error("cpr::Timeout: timeout value underflow: " +
                                     std::to_string(ms.count()) + " ms.");
      }

      return static_cast<long>(ms.count());
    }

    std::chrono::milliseconds ms;
};

} // namespace xxhr

#endif

#ifndef XXHR_BEARER_H
#define XXHR_BEARER_H

#include <string>

#include "defines.hpp"

namespace xxhr {

/**
 * \brief Some Web APIs requires authenticating via HTTP Basic auth ( *i.e.* base64 encoded user and password authentication).
 *
 * \copydoc authentication-cpp
 *
 */
class Bearer {
  public:

    //! Token for bearer authentication
    Bearer(const std::string& token)
            : auth_string_{token} {}

    const char* GetAuthString() const noexcept { return auth_string_.data(); }

  private:
    std::string auth_string_;
};

} // namespace xxhr

#endif

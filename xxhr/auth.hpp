#ifndef XXHR_AUTH_H
#define XXHR_AUTH_H

#include <string>

#include "defines.hpp"

namespace xxhr {

/**
 * \brief Some Web APIs requires authenticating via HTTP Digest auth ( *i.e.* base64 encoded user and password authentication).
 *
 * This can be done as follow : 
 * 
 * \snippet eamples/authentication.cpp Authentication-snippet
 *
 */
class Authentication {
  public:
    template <typename UserType, typename PassType>
    Authentication(UserType&& username, PassType&& password)
            : username_{XXHR_FWD(username)}, password_{XXHR_FWD(password)},
              auth_string_{username_ + ":" + password_} {}

    const char* GetAuthString() const noexcept { return auth_string_.data(); }

    std::string username() const { return username_; }
    std::string password() const { return password_; }

  private:
    std::string username_;
    std::string password_;
    std::string auth_string_;
};

} // namespace xxhr

#endif

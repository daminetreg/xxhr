#ifndef XXHR_AUTH_H
#define XXHR_AUTH_H

#include <string>

#include "defines.hpp"

namespace xxhr {

class Authentication {
  public:
    template <typename UserType, typename PassType>
    Authentication(UserType&& username, PassType&& password)
            : username_{XXHR_FWD(username)}, password_{XXHR_FWD(password)},
              auth_string_{username_ + ":" + password_} {}

    const char* GetAuthString() const noexcept { return auth_string_.data(); }

  private:
    std::string username_;
    std::string password_;
    std::string auth_string_;
};

} // namespace xxhr

#endif

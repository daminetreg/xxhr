#ifndef XXHR_AUTH_H
#define XXHR_AUTH_H

#include <string>

#include "defines.hpp"

namespace xxhr {

/**
 * \brief Some Web APIs requires authenticating via HTTP Basic auth ( *i.e.* base64 encoded user and password authentication).
 *
 * \copydoc authentication-cpp
 *
 */
class Authentication {
  public:

    //! Specify username and password for basic auth
    template <typename UserType, typename PassType>
    Authentication(UserType&& username, PassType&& password)
            : username_{XXHR_FWD(username)}, password_{XXHR_FWD(password)},
              auth_string_{username_ + ":" + password_} {}

    const char* GetAuthString() const noexcept { return auth_string_.data(); }

    //! 
    std::string username() const { return username_; }
    //! 
    std::string password() const { return password_; }

  private:
    std::string username_;
    std::string password_;
    std::string auth_string_;
};


/**
 * \brief Some Proxy requires a special kind of authentication via NTLM, this allows specifying it.
 */
class NTLM : public Authentication {
  public:
    NTLM(const std::string& username, const std::string& password)
            : Authentication{username, password} {}
};


} // namespace xxhr

#endif

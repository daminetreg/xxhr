#ifndef XXHR_DIGEST_H
#define XXHR_DIGEST_H

#include "auth.hpp"
#include "defines.hpp"

namespace xxhr {

/**
 * \brief Some Web APIs requires authenticating via HTTP Digest auth ( *i.e.* MD5 hashed user and password ).
 *
 * **Please note that this authentication mode as of xxhr v0.0.2 is supported only in WebAssembly backend.**
 *
 * This can be done as follow : 
 * 
 * \snippet examples/authentication.cpp Authentication-snippet-digest
 *
 */
class Digest : public Authentication {
  public:
    //!
    template <typename UserType, typename PassType>
    Digest(UserType&& username, PassType&& password)
            : Authentication{XXHR_FWD(username), XXHR_FWD(password)} {}

    const char* GetAuthString() const noexcept { return Authentication::GetAuthString(); }
};

} // namespace xxhr

#endif

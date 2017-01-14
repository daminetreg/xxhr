#ifndef XXHR_DIGEST_H
#define XXHR_DIGEST_H

#include "auth.hpp"
#include "defines.hpp"

namespace xxhr {

class Digest : public Authentication {
  public:
    template <typename UserType, typename PassType>
    Digest(UserType&& username, PassType&& password)
            : Authentication{XXHR_FWD(username), XXHR_FWD(password)} {}

    const char* GetAuthString() const noexcept { return Authentication::GetAuthString(); }
};

} // namespace xxhr

#endif

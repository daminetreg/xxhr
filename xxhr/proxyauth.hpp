#ifndef XXHR_PROXYAUTH_HPP
#define XXHR_PROXYAUTH_HPP

#include <initializer_list>
#include <map>

#include <xxhr/auth.hpp>
#include <xxhr/util.hpp>

namespace xxhr {
class EncodedAuthentication {
  public:
    EncodedAuthentication() : auth_string_{""} {}
    EncodedAuthentication(const std::string& username, const std::string& password) : auth_string_{util::urlEncode(username) + ":" + util::urlEncode(password)} {}
    EncodedAuthentication(std::string&& username, std::string&& password) : auth_string_{util::urlEncode(std::move(username)) + ":" + util::urlEncode(std::move(password))} {}
    EncodedAuthentication(const EncodedAuthentication& other) = default;
    EncodedAuthentication(EncodedAuthentication&& old) noexcept = default;
    virtual ~EncodedAuthentication() noexcept = default;

    EncodedAuthentication& operator=(EncodedAuthentication&& old) noexcept = default;
    EncodedAuthentication& operator=(const EncodedAuthentication& other) = default;

    const char* GetAuthString() const noexcept {
      return auth_string_.c_str();
    }

  protected:
    std::string auth_string_;
};

class ProxyAuthentication {
  public:
    ProxyAuthentication() = default;
    ProxyAuthentication(const std::initializer_list<std::pair<const std::string, EncodedAuthentication>>& auths) : proxyAuth_{auths} {}

    bool has(const std::string& protocol) const {
      return proxyAuth_.count(protocol) > 0;
    }

    const char* operator[](const std::string& protocol) {
      return proxyAuth_[protocol].GetAuthString();
    }

  private:
    std::map<std::string, EncodedAuthentication> proxyAuth_;
};

}

#endif
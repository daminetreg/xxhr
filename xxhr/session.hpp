#ifndef XXHR_SESSION_H
#define XXHR_SESSION_H

#include <cstdint>
#include <memory>

#include <xxhr/auth.hpp>
#include <xxhr/body.hpp>
#include <xxhr/cookies.hpp>
#include <xxhr/xxhrtypes.hpp>
#include <xxhr/digest.hpp>
#include <xxhr/max_redirects.hpp>
#include <xxhr/multipart.hpp>
#include <xxhr/parameters.hpp>
#include <xxhr/response.hpp>
#include <xxhr/timeout.hpp>
#include <xxhr/handler.hpp>
#include <xxhr/proxies.hpp>
#include <xxhr/proxyauth.hpp>

namespace xxhr {

class Session {
  public:
  inline Session();
  inline ~Session();

  inline void SetUrl(const Url& url);
  inline void SetParameters(const Parameters& parameters);
  inline void SetParameters(Parameters&& parameters);
  inline void SetHeader(const Header& header);
  inline void SetTimeout(const Timeout& timeout);
  inline void SetAuth(const Authentication& auth);
  inline void SetDigest(const Digest& auth);
  inline void SetBearer(const Bearer& auth);
  inline void SetProxies(Proxies&& proxies);
  inline void SetProxies(const Proxies& proxies);
  inline void SetProxyAuth(ProxyAuthentication&& proxy_auth);
  inline void SetProxyAuth(const ProxyAuthentication& proxy_auth);
  inline void SetMultipart(Multipart&& multipart);
  inline void SetMultipart(const Multipart& multipart);
  inline void SetNTLM(const NTLM& auth);
  inline void SetRedirect(const bool& redirect);
  inline void SetMaxRedirects(const MaxRedirects& max_redirects);
  inline void SetCookies(const Cookies& cookies);
  inline void SetBody(Body&& body);
  inline void SetBody(const Body& body);

  // Used in templated functions
  inline void SetOption(const Url& url);
  inline void SetOption(const Parameters& parameters);
  inline void SetOption(Parameters&& parameters);
  inline void SetOption(const Header& header);
  inline void SetOption(const Timeout& timeout);
  inline void SetOption(const Authentication& auth);
  inline void SetOption(const Digest& auth);
  inline void SetOption(const Bearer& auth);

  inline void SetOption(const Proxies& proxies);
  inline void SetOption(Proxies&& proxies);
  inline void SetOption(ProxyAuthentication&& proxy_auth);
  inline void SetOption(const ProxyAuthentication& proxy_auth);

  inline void SetOption(Multipart&& multipart);
  inline void SetOption(const Multipart& multipart);
  inline void SetOption(const NTLM& auth);
  inline void SetOption(const bool& redirect);
  inline void SetOption(const MaxRedirects& max_redirects);
  inline void SetOption(const Cookies& cookies);
  inline void SetOption(Body&& body);
  inline void SetOption(const Body& body);

  template<class Handler>
  inline void SetOption(const on_response_<Handler>&& on_response);

  inline void DELETE_();
  inline void GET();
  inline void HEAD();
  inline void OPTIONS();
  inline void PATCH();
  inline void POST();
  inline void PUT();

  class Impl;
  private:
  std::shared_ptr<Impl> pimpl_;
};

} // namespace xxhr

#include <xxhr/impl/session.hpp>

#endif

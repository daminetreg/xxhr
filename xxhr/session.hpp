#ifndef XXHR_SESSION_H
#define XXHR_SESSION_H

#include <cstdint>
#include <memory>

#include "auth.hpp"
#include "body.hpp"
#include "cookies.hpp"
#include "xxhrtypes.hpp"
#include "digest.hpp"
#include "max_redirects.hpp"
#include "multipart.hpp"
#include "parameters.hpp"
#include "payload.hpp"
#include "proxies.hpp"
#include "response.hpp"
#include "timeout.hpp"

namespace xxhr {

class Session {
  public:
  Session();
  ~Session();

  void SetUrl(const Url& url);
  void SetParameters(const Parameters& parameters);
  void SetParameters(Parameters&& parameters);
  void SetHeader(const Header& header);
  void SetTimeout(const Timeout& timeout);
  void SetAuth(const Authentication& auth);
  void SetDigest(const Digest& auth);
  void SetPayload(Payload&& payload);
  void SetPayload(const Payload& payload);
  void SetProxies(Proxies&& proxies);
  void SetProxies(const Proxies& proxies);
  void SetMultipart(Multipart&& multipart);
  void SetMultipart(const Multipart& multipart);
  void SetRedirect(const bool& redirect);
  void SetMaxRedirects(const MaxRedirects& max_redirects);
  void SetCookies(const Cookies& cookies);
  void SetBody(Body&& body);
  void SetBody(const Body& body);

  // Used in templated functions
  void SetOption(const Url& url);
  void SetOption(const Parameters& parameters);
  void SetOption(Parameters&& parameters);
  void SetOption(const Header& header);
  void SetOption(const Timeout& timeout);
  void SetOption(const Authentication& auth);
  void SetOption(const Digest& auth);
  void SetOption(Payload&& payload);
  void SetOption(const Payload& payload);
  void SetOption(Proxies&& proxies);
  void SetOption(const Proxies& proxies);
  void SetOption(Multipart&& multipart);
  void SetOption(const Multipart& multipart);
  void SetOption(const bool& redirect);
  void SetOption(const MaxRedirects& max_redirects);
  void SetOption(const Cookies& cookies);
  void SetOption(Body&& body);
  void SetOption(const Body& body);

  Response DELETE();
  Response GET();
  Response HEAD();
  Response OPTIONS();
  Response PATCH();
  Response POST();
  Response PUT();

  class Impl;
  private:
  std::shared_ptr<Impl> pimpl_;
};

} // namespace xxhr

#include <xxhr/impl/session.hpp>

#endif

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
#include "response.hpp"
#include "timeout.hpp"
#include "handler.hpp"

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
  void SetOption(Multipart&& multipart);
  void SetOption(const Multipart& multipart);
  void SetOption(const bool& redirect);
  void SetOption(const MaxRedirects& max_redirects);
  void SetOption(const Cookies& cookies);
  void SetOption(Body&& body);
  void SetOption(const Body& body);

  template<class Handler>
  void SetOption(const on_response_<Handler>&& on_response);

  void DELETE();
  void GET();
  void HEAD();
  void OPTIONS();
  void PATCH();
  void POST();
  void PUT();

  class Impl;
  private:
  std::shared_ptr<Impl> pimpl_;
};

} // namespace xxhr

#include <xxhr/impl/session.hpp>

#endif

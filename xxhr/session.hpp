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
  inline Session();
  inline ~Session();

  inline void SetUrl(const Url& url);
  inline void SetParameters(const Parameters& parameters);
  inline void SetParameters(Parameters&& parameters);
  inline void SetHeader(const Header& header);
  inline void SetTimeout(const Timeout& timeout);
  inline void SetAuth(const Authentication& auth);
  inline void SetDigest(const Digest& auth);
  inline void SetMultipart(Multipart&& multipart);
  inline void SetMultipart(const Multipart& multipart);
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
  inline void SetOption(Multipart&& multipart);
  inline void SetOption(const Multipart& multipart);
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

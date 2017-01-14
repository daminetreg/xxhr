#ifndef XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP
#define XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP

#include <boost/optional.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

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
#include "low_speed.hpp"

namespace xxhr {

  class Session::Impl {
    public:
      Impl();

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
      void SetLowSpeed(const LowSpeed& low_speed);

      Response DELETE();
      Response GET();
      Response HEAD();
      Response OPTIONS();
      Response PATCH();
      Response POST();
      Response PUT();

    private:
      Url url_;
      Parameters parameters_;

      // XXX: SHould be used in query open if not none.
      boost::optional<Auth> auth_;
      boost::optional<Digest> digest_;

      //! Payload to be given to xhr send.
      boost::optional<std::string> payload_;
      boost::optional<emscripten::val> formdata_;

      emscripten::val xhr = val::global("XMLHttpRequest").new_();
  };


  void Session::SetUrl(const Url& url) { url_ = url; }
  void Session::SetParameters(const Parameters& parameters) {
    parameters_ = parameters;
  }

  void Session::SetParameters(Parameters&& parameters) {
      parameters_ = std::move(parameters);
  }


  void Session::Impl::SetHeader(const Header& header) {
    for (auto item = header.cbegin(); item != header.cend(); ++item) {
      xhr.call<val>("setRequestHeader", item->first, item->second);
    }
  }

  void Session::Impl::SetTimeout(const Timeout& timeout) {
    xhr["timeout"] = timeout.ms.count();
  }

  void Session::Impl::SetAuth(const Authentication& auth) {
    auth_ = auth;
    //xhr.call<val>("setRequestHeader", 
    //  "Authorization", std::string("Basic ") + util::encode64(auth.GetAuthString());
  }

  void Session::Impl::SetDigest(const Digest& auth) {
    digest_ = auth;
    //xhr.call<val>("setRequestHeader", 
    //  "Authorization", std::string("Basic ") + util::encode64(auth.GetAuthString());
  }

  void Session::Impl::SetPayload(Payload&& payload) {
    xhr.call<val>("setRequestHeader", "Content-type", "application/x-www-form-urlencoded");
    payload_ = payload.content;
  }

  void Session::Impl::SetPayload(const Payload& payload) {
    SetPayload(std::move(payload));
  }

  void Session::Impl::SetProxies(const Proxies& ) { /* We cannot affect this in a webbrowser. Anyway there is nothing worse than http proxies. */ }
  void Session::Impl::SetProxies(Proxies&& ) { /* We cannot affect this in a webbrowser. Anyway there is nothing worse than http proxies.*/ }
  
  void Session::Impl::SetMultipart(Multipart&& multipart) {
    using emscripten::val;
    val formdata = val::global("FormData").new_();

    for (auto& part : multipart.parts) {
      // https://developer.mozilla.org/fr/docs/Web/API/FormData 
      if (part.is_file) {
        throw std::runtime_error("File multipart was not yet implemented, would require 1h of work though");
        //XXX: Implement file, File cannot be instantiated normally, it should be forwarded from user input.
        //https://developer.mozilla.org/en-US/docs/Web/API/File
        // XXX : An  idea would be to take the value.data() as a Javascript variable which contains a user selected
        // file, might see.
        //auto file = val::global("File").new_( part.value.data() );
        //buf["mimetype"] = part.content_type;
        //buf["size"] = part.value.size();

        //formdata.call<val>("append", buf );
      } else {
        auto buf = val::global("Blob").new_( part.value );
        if (!part.content_type.empty()) { buf["mimetype"] = part.content_type; }
        buf["size"] = part.value.size();

        formdata.call<val>("append", part.name, buf );
      }
    }

    formdata_ = formdata;
  }


  void Session::Impl::SetRedirect(const bool& ) {
    // TODO : Implement redirect with a switching backend, following support : 
    //  - Fetch API can handle redirection limitations
    //  - Fetch API is not yet supported on all browser
    //  - If not then throw or indicate with some error message and go back to xhr
    // https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch
  }

  void Session::Impl::SetMaxRedirects(const MaxRedirects& ) {
    // TODO : Implement redirect with a switching backend, following support : 
    //  - Fetch API can handle redirection limitations
    //  - Fetch API is not yet supported on all browser
    //  - If not then throw or indicate with some error message and go back to xhr
    // https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API/Using_Fetch
  }

  void Session::Impl::SetCookies(const Cookies& cookies) {
    xhr["withCredentials"] = true;

    auto document = val::global("document");
    for (auto cookie : cookies.all()) {
      document["cookie"] = 
        xxhr::util::urlEncode(cookie.first)
        + "=" + 
        xxhr::util::urlEncode(cookie.second)
      ;
    }

    // TODO: after sending remove them from document with
    // document.cookie = "username=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;";  
  }



  Session::Session() : pimpl_{ std::make_unique<Impl>() } {}
  Session::~Session() {}
  void Session::SetUrl(const Url& url) { pimpl_->SetUrl(url); }
  void Session::SetParameters(const Parameters& parameters) { pimpl_->SetParameters(parameters); }
  void Session::SetParameters(Parameters&& parameters) { pimpl_->SetParameters(std::move(parameters)); }
  void Session::SetHeader(const Header& header) { pimpl_->SetHeader(header); }
  void Session::SetTimeout(const Timeout& timeout) { pimpl_->SetTimeout(timeout); }
  void Session::SetAuth(const Authentication& auth) { pimpl_->SetAuth(auth); }
  void Session::SetDigest(const Digest& auth) { pimpl_->SetDigest(auth); }
  void Session::SetPayload(const Payload& payload) { pimpl_->SetPayload(payload); }
  void Session::SetPayload(Payload&& payload) { pimpl_->SetPayload(std::move(payload)); }
  void Session::SetProxies(const Proxies& proxies) { pimpl_->SetProxies(proxies); }
  void Session::SetProxies(Proxies&& proxies) { pimpl_->SetProxies(std::move(proxies)); }
  void Session::SetMultipart(const Multipart& multipart) { pimpl_->SetMultipart(multipart); }
  void Session::SetMultipart(Multipart&& multipart) { pimpl_->SetMultipart(std::move(multipart)); }
  void Session::SetRedirect(const bool& redirect) { pimpl_->SetRedirect(redirect); }
  void Session::SetMaxRedirects(const MaxRedirects& max_redirects) { pimpl_->SetMaxRedirects(max_redirects); }
  void Session::SetCookies(const Cookies& cookies) { pimpl_->SetCookies(cookies); }
  void Session::SetBody(const Body& body) { pimpl_->SetBody(body); }
  void Session::SetBody(Body&& body) { pimpl_->SetBody(std::move(body)); }
  void Session::SetLowSpeed(const LowSpeed& low_speed) { pimpl_->SetLowSpeed(low_speed); }
  void Session::SetOption(const Url& url) { pimpl_->SetUrl(url); }
  void Session::SetOption(const Parameters& parameters) { pimpl_->SetParameters(parameters); }
  void Session::SetOption(Parameters&& parameters) { pimpl_->SetParameters(std::move(parameters)); }
  void Session::SetOption(const Header& header) { pimpl_->SetHeader(header); }
  void Session::SetOption(const Timeout& timeout) { pimpl_->SetTimeout(timeout); }
  void Session::SetOption(const Authentication& auth) { pimpl_->SetAuth(auth); }
  void Session::SetOption(const Digest& auth) { pimpl_->SetDigest(auth); }
  void Session::SetOption(const Payload& payload) { pimpl_->SetPayload(payload); }
  void Session::SetOption(Payload&& payload) { pimpl_->SetPayload(std::move(payload)); }
  void Session::SetOption(const Proxies& proxies) { pimpl_->SetProxies(proxies); }
  void Session::SetOption(Proxies&& proxies) { pimpl_->SetProxies(std::move(proxies)); }
  void Session::SetOption(const Multipart& multipart) { pimpl_->SetMultipart(multipart); }
  void Session::SetOption(Multipart&& multipart) { pimpl_->SetMultipart(std::move(multipart)); }
  void Session::SetOption(const bool& redirect) { pimpl_->SetRedirect(redirect); }
  void Session::SetOption(const MaxRedirects& max_redirects) { pimpl_->SetMaxRedirects(max_redirects); }
  void Session::SetOption(const Cookies& cookies) { pimpl_->SetCookies(cookies); }
  void Session::SetOption(const Body& body) { pimpl_->SetBody(body); }
  void Session::SetOption(Body&& body) { pimpl_->SetBody(std::move(body)); }
  void Session::SetOption(const LowSpeed& low_speed) { pimpl_->SetLowSpeed(low_speed); }
  Response Session::DELETE() { return pimpl_->DELETE(); }
  Response Session::GET() { return pimpl_->GET(); }
  Response Session::HEAD() { return pimpl_->HEAD(); }
  Response Session::OPTIONS() { return pimpl_->OPTIONS(); }
  Response Session::PATCH() { return pimpl_->PATCH(); }
  Response Session::POST() { return pimpl_->POST(); }
  Response Session::PUT() { return pimpl_->PUT(); }

}

#endif // XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP

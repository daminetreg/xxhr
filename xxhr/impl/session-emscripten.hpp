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

namespace xxhr {

  class Session::Impl {
    using emscripten::val;

    public:
      Impl();

      void SetUrl(const Url& url);
      void SetParameters(const Parameters& parameters);
      void SetParameters(Parameters&& parameters);
      void SetHeader(const Header& header);
      void SetTimeout(const Timeout& timeout);
      void SetAuth(const Authentication& auth);
      void SetDigest(const Digest& auth);

      //! Set the body of the request url encoded
      void SetPayload(Payload&& payload);
      
      //! Set the body of the request url encoded
      void SetPayload(const Payload& payload);
      void SetProxies(Proxies&& proxies);
      void SetProxies(const Proxies& proxies);
      void SetMultipart(Multipart&& multipart);
      void SetMultipart(const Multipart& multipart);
      void SetRedirect(const bool& redirect);
      void SetMaxRedirects(const MaxRedirects& max_redirects);
      void SetCookies(const Cookies& cookies, bool delete_them = false);
      void CookiesCleanup():

      //! Set the provided body of request raw, without urlencoding
      void SetBody(Body&& body);
      
      //! Set the provided body of request raw, without urlencoding
      void SetBody(const Body& body);

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
      Cookies cookies_;

      // XXX: SHould be used in query open if not none.
      boost::optional<Auth> auth_;

      //! Payload to be given to xhr send.
      boost::optional<std::string> body_;
      boost::optional<val> multipart_;

      val xhr = val::global("XMLHttpRequest").new_();

      void on_load(val event) { 
        std::cout << "Successful Query " << std::endl;
        std::cout << "response is : " << xhr["responseText"].as<std::string>() << std::endl;
      }

      void on_error(val event) {
        std::cout << "Error on query " << std::endl;
      }

      void on_progress(val event) {
        std::cout << "Progress on query " << std::endl;

        std::cout << event["lengthComputable"].as<bool>() << ": " << event["loaded"].as<unsigned int>() / event["total"].as<unsigned int>() << std::endl;
      }

  };

  EMSCRIPTEN_BINDINGS(SessionImpl) {
    class_<>("xxhr_SessionImpl")
      .constructor()
      .function("on_load", &Session::Impl::on_load)
      .function("on_error", &Session::Impl::on_error)
      .function("on_progress", &Session::Impl::on_progress)
      ;
  }

  void Session::Impl::SetUrl(const Url& url) { url_ = url; }
  void Session::Impl::SetParameters(const Parameters& parameters) {
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
    auth_ = auth;
    //xhr.call<val>("setRequestHeader", 
    //  "Authorization", std::string("Basic ") + util::encode64(auth.GetAuthString());
  }

  void Session::Impl::SetPayload(Payload&& payload) {
    xhr.call<val>("setRequestHeader", "Content-type", "application/x-www-form-urlencoded");
    body_ = payload.content;
  }

  void Session::Impl::SetPayload(const Payload& payload) {
    SetPayload(std::move(payload));
  }

  void Session::Impl::SetProxies(const Proxies& ) { /* We cannot affect this in a webbrowser. Anyway there is nothing worse than http proxies. */ }
  void Session::Impl::SetProxies(Proxies&& ) { /* We cannot affect this in a webbrowser. Anyway there is nothing worse than http proxies.*/ }
  
  void Session::Impl::SetMultipart(Multipart&& multipart) {
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

    multipart_ = formdata;
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

  void Session::Impl::SetCookies(const Cookies& cookies, bool delete_them) {
    cookies_ = cookies;
    xhr["withCredentials"] = true;

    auto document = val::global("document");
    for (auto cookie : cookies.all()) {
      auto cookie_string = 
        xxhr::util::urlEncode(cookie.first)
        + "=" + 
        xxhr::util::urlEncode(cookie.second);

      if (delete_them) { 
        cookie_string += "; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;"; 
        // If someone finds this a good programming idiom he should go to hell
        // with all their browser developers friends. Instead of giving you
        // cookie.delete, we only have got the possibility to put a date in the
        // past... -_-'
      } 
      
      // This [de]register the cookies, but if you just read cookie you 
      // get the full list. -_-'
      document["cookie"] = cookie_string; 
    }

  }

  //! After sending remove request specific cookies from global document
  void Session::Impl::CookiesCleanup() {
    SetCookies(cookies_, true);
  }

  void Session::Impl::SetBody(Body&& body) { body_ = body; }
  void Session::Impl::SetBody(const Body& body) { body_ = body; }

  Response Session::Impl::DELETE() {

    if (auth_) {
      xhr.call<val>("open", std::string("DELETE"), url_, true, 
        auth_->user(), auth_->password() );
    } else {
      xhr.call<val>("open", std::string("DELETE"), url_, true);
    }


  }

  Response Session::Impl::GET() { 
    xhr["onload"] = val(*this)["on_load"];
    xhr["onerror"] = val(*this)["on_error"];
    xhr["onprogress"] = val(*this)["on_progress"];

    if (auth_) {
      xhr.call<val>("open",
        std::string("GET"), url_, true, auth_->user(), auth_->password() );
    } else {
      xhr.call<val>("open", 
        std::string("GET"), url_, true);
    }

    xhr.call<val>("send");
  }

  Response Session::Impl::HEAD() { return Response{}; }
  Response Session::Impl::OPTIONS() { return Response{}; }
  Response Session::Impl::PATCH() { return Response{}; }
  Response Session::Impl::POST() { return Response{}; }
  Response Session::Impl::PUT() { return Response{}; }



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
  Response Session::DELETE() { return pimpl_->DELETE(); }
  Response Session::GET() { return pimpl_->GET(); }
  Response Session::HEAD() { return pimpl_->HEAD(); }
  Response Session::OPTIONS() { return pimpl_->OPTIONS(); }
  Response Session::PATCH() { return pimpl_->PATCH(); }
  Response Session::POST() { return pimpl_->POST(); }
  Response Session::PUT() { return pimpl_->PUT(); }

}

#endif // XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP

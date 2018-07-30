#ifndef XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP
#define XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP

#include <functional>
#include <regex>

#include <optional>
#include <boost/algorithm/string/trim_all.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <js/bind.hpp>

#include <iostream>

#include <xxhr/auth.hpp>
#include <xxhr/body.hpp>
#include <xxhr/cookies.hpp>
#include <xxhr/xxhrtypes.hpp>
#include <xxhr/digest.hpp>
#include <xxhr/max_redirects.hpp>
#include <xxhr/multipart.hpp>
#include <xxhr/parameters.hpp>
#include <xxhr/payload.hpp>
#include <xxhr/proxies.hpp>
#include <xxhr/response.hpp>
#include <xxhr/timeout.hpp>

namespace xxhr {

  class Session::Impl : public std::enable_shared_from_this<Session::Impl> {
    using val = emscripten::val;
    using str = std::string;

    public:

    void SetUrl(const Url& url);
    void SetParameters(const Parameters& parameters);
    void SetParameters(Parameters&& parameters);
    void SetHeader(const Header& header);
    void SetTimeout(const Timeout& timeout);
    void SetAuth(const Authentication& auth);
    void SetDigest(const Digest& auth);

    void SetProxies(Proxies&& proxies);
    void SetProxies(const Proxies& proxies);
    void SetMultipart(Multipart&& multipart);
    void SetMultipart(const Multipart& multipart);
    void SetRedirect(const bool& redirect);
    void SetMaxRedirects(const MaxRedirects& max_redirects);
    void SetCookies(const Cookies& cookies, bool delete_them = false);
    void CookiesCleanup();

    //! Set the provided body
    void SetBody(const Body& body);

    template<class Handler>
    void SetHandler(const on_response_<Handler>&& functor) {
      on_response = functor;
    }

    void QUERY(const std::string& method);

    void DELETE();
    void GET();
    void HEAD();
    void OPTIONS();
    void PATCH();
    void POST();
    void PUT();

    enum ReadyState {
      UNSENT = 0, // 	Client has been created. open() not called yet.
      OPENED, // 	open() has been called.
      HEADERS_RECEIVED, //	send() has been called, and headers and status are available.
      LOADING, // 	Downloading; responseText holds partial data.
      DONE
    };

    void on_readystate(val event) {

      if (xhr["readyState"].as<size_t>() == DONE) {

        auto document = val::global("document");
        Cookies response_cookies{};
        response_cookies
          .parse_cookie_string(document["cookie"].as<std::string>());

        CookiesCleanup();

        on_response(
            Response{
              xhr["status"].as<size_t>(),
              (xhr["status"].as<size_t>() == 0) ?
                  Error{ErrorCode::CONNECTION_FAILURE} 
                : Error{},
              xhr["responseText"].as<std::string>(),
              (!xhr.call<val>("getAllResponseHeaders").isNull()) ?
                  parse_raw_headers( xhr.call<std::string>("getAllResponseHeaders"))
                : Header{}
              ,
              url_,
              response_cookies
            }
        );


      } else if (xhr["readyState"].as<size_t>() == LOADING) {
        std::cout << "xxhr::on_readystate: LOADING. " << url_ << " :: " << " " << xhr["status"].as<size_t>() << ": " << xhr["statusText"].as<std::string>() << std::endl;
        std::cout << "Partial data received: " << xhr["statusText"].as<std::string>().size() << std::endl;
      }

    }

    private:
    Url url_;
    Parameters parameters_; // TODO: implement parameters
    Cookies cookies_;
    Header headers;

    // XXX: SHould be used in query open if not none.
    std::optional<Authentication> auth_;

    //! Payload to be given to xhr send.
    boost::optional<std::string> body_;
    boost::optional<val> multipart_;

    val xhr = val::global("XMLHttpRequest").new_();

    std::function<void(Response&&)> on_response;

    Header parse_raw_headers(const std::string& headers_raw) const;
  };


  using emscripten::class_;
  using emscripten::function;
  using emscripten::select_overload;
  //EMSCRIPTEN_BINDINGS(SessionImpl) {
  //  class_<Session::Impl>("xxhr_SessionImpl")
  //    .smart_ptr<std::shared_ptr<Session::Impl>>("shared_ptr<xxhr_SessionImpl>")
  //    ;
  //}

  void Session::Impl::SetUrl(const Url& url) { url_ = url; }
  void Session::Impl::SetParameters(Parameters&& parameters) {
      parameters_ = std::move(parameters);
  }
  void Session::Impl::SetParameters(const Parameters& parameters) {
    parameters_ = parameters;
  }


  void Session::Impl::SetHeader(const Header& header) {
    headers.insert(header.begin(), header.end());
  }

  void Session::Impl::SetTimeout(const Timeout& timeout) {
    xhr["timeout"] = val(timeout.ms.count());
  }

  void Session::Impl::SetAuth(const Authentication& auth) {
    auth_ = auth;
    //Some old browser might need this, instead of open with pwd : xhr.call<val>("setRequestHeader", 
    //  "Authorization", std::string("Basic ") + util::encode64(auth.GetAuthString());
  }

  void Session::Impl::SetDigest(const Digest& auth) {
    auth_ = auth;
    //Some old browser might need this, instead of open with pwd : xhr.call<val>("setRequestHeader", 
    //  "Authorization", std::string("Basic ") + util::encode64(auth.GetAuthString());
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
      } else if (part.is_buffer) {
        auto buf = val::global("Blob").new_( part.value );
        if (!part.content_type.empty()) { buf["mimetype"] = val(part.content_type); }
        buf["size"] = val(part.value.size());

        formdata.call<val>("append", part.name, buf );
      } 

      // TODO : Implement normal form text value for web platform
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
    // XXX: Should we before we set any specified cookies, we have to remove ALL others ?
    
    cookies_ = cookies;
    xhr["withCredentials"] = val(true);

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
      auto document = val::global("document");
      document["cookie"] = val(cookie_string); 
    }

  }

  //! After sending remove request specific cookies from global document
  void Session::Impl::CookiesCleanup() { SetCookies(cookies_, true); }

  void Session::Impl::SetBody(const Body& body) { 

    if (body.is_form_encoded) {
      xhr.call<val>("setRequestHeader", str("Content-type"), str("application/x-www-form-urlencoded"));
    }

    body_ = body.content; 
  }

  void Session::Impl::QUERY(const std::string& method) {
    using namespace std::placeholders;

    if (auth_) {
      xhr.call<val>("open", method, url_, true, auth_->username(), auth_->password() );
    } else {
      xhr.call<val>("open", method, url_, true);
    }
    
    for (auto item = headers.cbegin(); item != headers.cend(); ++item) {
      xhr.call<val>("setRequestHeader", item->first, item->second);
    }

    xhr.set("onreadystatechange", js::bind(&Session::Impl::on_readystate, shared_from_this(), _1));

    if (body_) {
      xhr.call<val>("send", *body_);
    } else if (multipart_) {
      xhr.call<val>("send", *multipart_);
    } else {
      xhr.call<val>("send");
    }

  }

  void Session::Impl::DELETE()  { this->QUERY("DELETE"); }
  void Session::Impl::GET()     { this->QUERY("GET"); }
  void Session::Impl::HEAD()    { this->QUERY("HEAD"); }
  void Session::Impl::OPTIONS() { this->QUERY("OPTIONS"); }
  void Session::Impl::PATCH()   { this->QUERY("PATCH"); }
  void Session::Impl::POST()    { this->QUERY("POST"); }
  void Session::Impl::PUT()     { this->QUERY("PUT"); }


  Header Session::Impl::parse_raw_headers(const std::string& headers_raw) const {
    Header headers;
    using namespace boost::algorithm;
    std::regex crlfs(CRLF);
    std::vector<std::string> header_lines(
      std::sregex_token_iterator(
        headers_raw.begin(), headers_raw.end(), crlfs, -1),
      std::sregex_token_iterator()
    );

    for (auto& header_line : header_lines) {
      std::regex doublepoint(":");
      std::vector<std::string> header(
        std::sregex_token_iterator(
          header_line.begin(), header_line.end(), doublepoint, -1),
        std::sregex_token_iterator()
      );

      trim_all(header[0]); trim_all(header[1]);
      headers.insert(std::make_pair(header[0], header[1]));
    }

    return headers;
  }


  Session::Session() : pimpl_{ std::make_shared<Impl>() } {}
  Session::~Session() {}
  void Session::SetUrl(const Url& url) { pimpl_->SetUrl(url); }
  void Session::SetParameters(const Parameters& parameters) { pimpl_->SetParameters(parameters); }
  void Session::SetParameters(Parameters&& parameters) { pimpl_->SetParameters(std::move(parameters)); }
  void Session::SetHeader(const Header& header) { pimpl_->SetHeader(header); }
  void Session::SetTimeout(const Timeout& timeout) { pimpl_->SetTimeout(timeout); }
  void Session::SetAuth(const Authentication& auth) { pimpl_->SetAuth(auth); }
  void Session::SetDigest(const Digest& auth) { pimpl_->SetDigest(auth); }
  void Session::SetProxies(const Proxies& proxies) { pimpl_->SetProxies(proxies); }
  void Session::SetProxies(Proxies&& proxies) { pimpl_->SetProxies(std::move(proxies)); }
  void Session::SetMultipart(const Multipart& multipart) { pimpl_->SetMultipart(multipart); }
  void Session::SetMultipart(Multipart&& multipart) { pimpl_->SetMultipart(std::move(multipart)); }
  void Session::SetRedirect(const bool& redirect) { pimpl_->SetRedirect(redirect); }
  void Session::SetMaxRedirects(const MaxRedirects& max_redirects) { pimpl_->SetMaxRedirects(max_redirects); }
  void Session::SetCookies(const Cookies& cookies) { pimpl_->SetCookies(cookies); }
  void Session::SetBody(const Body& body) { pimpl_->SetBody(body); }
  void Session::SetBody(Body&& body) { pimpl_->SetBody(body); }
  void Session::SetOption(const Url& url) { pimpl_->SetUrl(url); }
  void Session::SetOption(const Parameters& parameters) { pimpl_->SetParameters(parameters); }
  void Session::SetOption(Parameters&& parameters) { pimpl_->SetParameters(std::move(parameters)); }
  void Session::SetOption(const Header& header) { pimpl_->SetHeader(header); }
  void Session::SetOption(const Timeout& timeout) { pimpl_->SetTimeout(timeout); }
  void Session::SetOption(const Authentication& auth) { pimpl_->SetAuth(auth); }
  void Session::SetOption(const Digest& auth) { pimpl_->SetDigest(auth); }
  void Session::SetOption(const Proxies& proxies) { pimpl_->SetProxies(proxies); }
  void Session::SetOption(Proxies&& proxies) { pimpl_->SetProxies(std::move(proxies)); }
  void Session::SetOption(const Multipart& multipart) { pimpl_->SetMultipart(multipart); }
  void Session::SetOption(Multipart&& multipart) { pimpl_->SetMultipart(std::move(multipart)); }
  void Session::SetOption(const bool& redirect) { pimpl_->SetRedirect(redirect); }
  void Session::SetOption(const MaxRedirects& max_redirects) { pimpl_->SetMaxRedirects(max_redirects); }
  void Session::SetOption(const Cookies& cookies) { pimpl_->SetCookies(cookies); }
  void Session::SetOption(const Body& body) { pimpl_->SetBody(body); }
  void Session::SetOption(Body&& body) { pimpl_->SetBody(body); }

  template<class Handler>
  void Session::SetOption(const on_response_<Handler>&& on_response) {pimpl_->SetHandler(std::move(on_response)); }

  void Session::DELETE()  { pimpl_->DELETE(); }
  void Session::GET()     { pimpl_->GET(); }
  void Session::HEAD()    { pimpl_->HEAD(); }
  void Session::OPTIONS() { pimpl_->OPTIONS(); }
  void Session::PATCH()   { pimpl_->PATCH(); }
  void Session::POST()    { pimpl_->POST(); }
  void Session::PUT()     { pimpl_->PUT(); }

}

#endif // XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP

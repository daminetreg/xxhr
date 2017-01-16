#ifndef XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP
#define XXHR_DETAIL_SESSION_EMSCRIPTEN_HPP

#include <functional>

#include <boost/optional.hpp>

#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>

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
      void CookiesCleanup();

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

      enum ReadyState {
        UNSENT = 0, // 	Client has been created. open() not called yet.
        OPENED, // 	open() has been called.
        HEADERS_RECEIVED, //	send() has been called, and headers and status are available.
        LOADING, // 	Downloading; responseText holds partial data.
        DONE
      };

      void on_readystate() {
      }

      void on_load(val event) { 
        std::cout << "Successful Query on " << url_ << std::endl;
        CookiesCleanup();
        std::cout << "response is : " << (xhr["responseText"]).as<std::string>() << std::endl;
      }

      void on_error(val) {
        std::cout << "Error on query " << std::endl;
        CookiesCleanup();
      }

      void on_progress(val event) {
        std::cout << "Progress on query " << std::endl;

        std::cout << event["lengthComputable"].as<bool>() << ": " << event["loaded"].as<unsigned int>() / event["total"].as<unsigned int>() << std::endl;
      }

      static std::shared_ptr<Impl> current_instance_static_storage(std::shared_ptr<Impl> value = std::shared_ptr<Impl>{}) {
        static std::shared_ptr<Impl> current_instance_;
        if (value.get() != nullptr) {
          current_instance_ = value;
          return current_instance_;
        } else {
          std::shared_ptr<Impl> return_;
          return_.swap(current_instance_); 
          return return_;
        }
      }

      static std::shared_ptr<Impl> current_instance() {
        return current_instance_static_storage();
      }

    private:
      Url url_;
      Parameters parameters_;
      Cookies cookies_;

      // XXX: SHould be used in query open if not none.
      boost::optional<Authentication> auth_;

      //! Payload to be given to xhr send.
      boost::optional<std::string> body_;
      boost::optional<val> multipart_;

      val xhr = val::global("XMLHttpRequest").new_();

      friend struct EmscriptenBindingInitializer_SessionImpl; 
  };


  using emscripten::class_;
  using emscripten::function;
  using emscripten::select_overload;
  EMSCRIPTEN_BINDINGS(SessionImpl) {
    class_<Session::Impl>("xxhr_SessionImpl")
      .smart_ptr<std::shared_ptr<Session::Impl>>("shared_ptr<xxhr_SessionImpl>")
      .constructor<>()
      //.smart_ptr_constructor("xxhr_SessionImplSmartPtr", &std::make_shared<Session::Impl>)
      .function("on_load", &Session::Impl::on_load)
      .function("on_error", &Session::Impl::on_error)
      .function("on_progress", &Session::Impl::on_progress)
      .property("xhr", &Session::Impl::xhr)
      ;
    function("SessionImpl_current_instance", select_overload< std::shared_ptr<Session::Impl>() >(&Session::Impl::current_instance) );//, allow_raw_pointers());


    class_<std::function<void(emscripten::val)>>("VoidValFunctor")
      .constructor<>()
      .function("opcall", &std::function<void(emscripten::val)>::operator());
  }





//  struct Interface {
//      virtual void invoke(const std::string& str) = 0;
//  };
//
//  struct InterfaceWrapper : public wrapper<Interface> {
//      EMSCRIPTEN_WRAPPER(InterfaceWrapper);
//      void invoke(const std::string& str) {
//          return call<void>("invoke", str);
//      }
//  };
//
//  EMSCRIPTEN_BINDINGS(interface) {
//      class_<Interface>("Interface")
//          .function("invoke", &Interface::invoke, pure_virtual())
//          .allow_subclass<InterfaceWrapper>("InterfaceWrapper")
//          ;
//  }





  void Session::Impl::SetUrl(const Url& url) { url_ = url; }
  void Session::Impl::SetParameters(Parameters&& parameters) {
      parameters_ = std::move(parameters);
  }
  void Session::Impl::SetParameters(const Parameters& parameters) {
    parameters_ = parameters;
  }


  void Session::Impl::SetHeader(const Header& header) {
    for (auto item = header.cbegin(); item != header.cend(); ++item) {
      xhr.call<val>("setRequestHeader", item->first, item->second);
    }
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

  void Session::Impl::SetPayload(Payload&& payload) {
    xhr.call<val>("setRequestHeader", str("Content-type"), str("application/x-www-form-urlencoded"));
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
        if (!part.content_type.empty()) { buf["mimetype"] = val(part.content_type); }
        buf["size"] = val(part.value.size());

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
    xhr["withCredentials"] = val(true);

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
      document["cookie"] = val(cookie_string); 
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
        auth_->username(), auth_->password() );
    } else {
      xhr.call<val>("open", std::string("DELETE"), url_, true);
    }


    return Response{};
  }

  Response Session::Impl::GET() { 
    if (auth_) {
      xhr.call<val>("open",
        std::string("GET"), url_, true, auth_->username(), auth_->password() );
    } else {
      xhr.call<val>("open", 
        std::string("GET"), url_, true);
    }

    emscripten_run_script(R"( 
      stdutils = {
        stdfunction_bind : function(functor) {
          return function () {
            return functor.opcall.apply(functor, arguments); 
          };
        }
      };
    )");
    
    //current_instance_static_storage(shared_from_this());
    //val::global("centraldispatch").call<val>("set_xhr_callbacks");

    //xhr.set("onload", val(std::bind(&Session::Impl::on_load, this, std::placeholders::_1));//this_in_js["on_load"];
    //val::global("centraldispatch")["test"] = val(shared_from_this())["on_load"];
    //auto some  = val::global("centraldispatch");
    //std::string hello = "fbewewibfk";
    //some.set("cool", val(hello));
    //some.set("fun", val(shared_from_this()));

    //!!! this is not bound xhr.set("onload", val(val(shared_from_this())["on_load"]));
   
    {
      std::function<void(emscripten::val)> functor = std::bind(&Session::Impl::on_load, shared_from_this(), std::placeholders::_1);
      //val functor_adapter = val::global("stdutils").call<val>("stdfunction_bind", val(functor));
      auto functor_adapter = val(functor)["opcall"].call<val>("bind", val(functor));
      xhr.set("onload", functor_adapter);
    }

    {
      std::function<void(emscripten::val)> functor = std::bind(&Session::Impl::on_error, this, std::placeholders::_1);
      val functor_adapter = val::global("stdutils").call<val>("stdfunction_bind", val(functor));
      xhr.set("onerror", functor_adapter);
    }
    //xhr.set("onload", this_in_js["on_load"]);
//    xhr["onprogress"] = this_in_js["on_progress"];

    //EM_ASM_(}
    //  $0.xhr.onload = $0.on_load; 
    //  $0.xhr.onerror = $0.on_error; 
    //}, this);

    xhr.call<val>("send");
    return Response{};
  }

  Response Session::Impl::HEAD() { return Response{}; }
  Response Session::Impl::OPTIONS() { return Response{}; }
  Response Session::Impl::PATCH() { return Response{}; }
  Response Session::Impl::POST() { return Response{}; }
  Response Session::Impl::PUT() { return Response{}; }



  Session::Session() : pimpl_{ std::make_shared<Impl>() } {}
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

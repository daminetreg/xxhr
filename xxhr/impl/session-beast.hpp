#ifndef XXHR_DETAIL_SESSION_BEAST_HPP
#define XXHR_DETAIL_SESSION_BEAST_HPP

#include <functional>
#include <regex>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include <boost/optional.hpp>
#include <boost/algorithm/string/trim_all.hpp>

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

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>


namespace xxhr::detail {

//
// Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/beast
//

//------------------------------------------------------------------------------
//
// Example: HTTP SSL client, asynchronous
//
//------------------------------------------------------------------------------



using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

//------------------------------------------------------------------------------

// Report a failure
inline void fail(boost::system::error_code ec, char const* what) { 
  std::cerr << what << ": " << ec.message() << "\n";
}


namespace xxhr {

  class Session::Impl : public std::enable_shared_from_this<Session::Impl> {
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

    private:
    http::request<http::string_body> req_;
    std::chrono::millisecond timeout_;

    std::function<void(Response&&)> on_response;

    boost::asio::io_context ioc;
    ssl::context ctx{ssl::context::sslv23_client};
    tcp::resolver resolver_{ioc};
    ssl::stream<tcp::socket> stream_{ioc, ctx};
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    http::response<http::string_body> res_;

    // Start the asynchronous operation
    void
    run(
        char const* host,
        char const* port,
        char const* target,
        int version)
    {
        // Set SNI Hostname (many hosts need this to handshake successfully)
        // XXX: openssl specificae, abstract this please
        if(! SSL_set_tlsext_host_name(stream_.native_handle(), host))
        {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            std::cerr << ec.message() << "\n";
            return;
        }

        // Set up an HTTP GET request message
        req_.version(version);
        req_.method(http::verb::get);
        req_.target(target);
        req_.set(http::field::host, host);
        req_.set(http::field::user_agent, "octoxxit/v0.0.1");

        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            std::bind(
                &session::on_resolve,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void
    on_resolve(
        boost::system::error_code ec,
        tcp::resolver::results_type results)
    {
        if(ec)
            return fail(ec, "resolve");

        // Make the connection on the IP address we get from a lookup
        boost::asio::async_connect(
            stream_.next_layer(),
            results.begin(),
            results.end(),
            std::bind(
                &session::on_connect,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_connect(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "connect");

        // Perform the SSL handshake
        stream_.async_handshake(
            ssl::stream_base::client,
            std::bind(
                &session::on_handshake,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_handshake(boost::system::error_code ec)
    {
        if(ec)
            return fail(ec, "handshake");

        // Send the HTTP request to the remote host
        http::async_write(stream_, req_,
            std::bind(
                &session::on_write,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void
    on_write(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "write");
        
        // Receive the HTTP response
        http::async_read(stream_, buffer_, res_,
            std::bind(
                &session::on_read,
                shared_from_this(),
                std::placeholders::_1,
                std::placeholders::_2));
    }

    void
    on_read(
        boost::system::error_code ec,
        std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if(ec)
            return fail(ec, "read");

        // Write the message to standard out
        std::cout << res_ << std::endl;

        on_response(res_.body());

        // Gracefully close the stream
        stream_.async_shutdown(
            std::bind(
                &session::on_shutdown,
                shared_from_this(),
                std::placeholders::_1));
    }

    void
    on_shutdown(boost::system::error_code ec)
    {
        if(ec == boost::asio::error::eof)
        {
            // Rationale:
            // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
            ec.assign(0, ec.category());
        }
        if(ec)
            return fail(ec, "shutdown");

        // If we get here then the connection is closed gracefully
    }


  };

  void Session::Impl::SetUrl(const Url& url) { url_ = url; }
  void Session::Impl::SetParameters(Parameters&& parameters) {
    parameters_ = std::move(parameters);
  }
  void Session::Impl::SetParameters(const Parameters& parameters) {
    parameters_ = parameters;
  }

  void Session::Impl::SetHeader(const Header& header) {
    for (auto&& entry : header) {
      auto& h = entry.first;
      auto& v = entry.second;
      req_.set(boost::beast::http::string_to_field(h), v);
    }
  }

  void Session::Impl::SetTimeout(const Timeout& timeout) {
    timeout_ = timeout.ms;
  }

  void Session::Impl::SetAuth(const Authentication& auth) {
    namespace http = boost::beast::http;
    req_.set(http::field::authorization, util::encode64(auth.GetAuthString()));
  }

  void Session::Impl::SetDigest(const Digest& auth) {
    namespace http = boost::beast::http;
    req_.set(http::field::authorization, util::encode64(auth.GetAuthString()));
  }

  void Session::Impl::SetPayload(Payload&& payload) {
    req_.set(http::field::content_type, "application/x-www-form-urlencoded");
    req_.body = payload.content;
  }

  void Session::Impl::SetPayload(const Payload& payload) {
    SetPayload(std::move(const_cast<Payload&>(payload)));
  }

  void Session::Impl::SetProxies(const Proxies& ) { 
    /* TODO: there isn't anything worse than http proxies. */ }
  void Session::Impl::SetProxies(Proxies&& ) { 
    /* TODO: there isn't anything worse than http proxies. */ }
 


/*
 
Upgrade-Insecure-Requests: 1
Content-Type: multipart/form-data; boundary=---------------------------8721656041911415653955004498
Content-Length: 465

-----------------------------8721656041911415653955004498
Content-Disposition: form-data; name="myTextField"

Test
-----------------------------8721656041911415653955004498
Content-Disposition: form-data; name="myCheckBox"

on
-----------------------------8721656041911415653955004498
Content-Disposition: form-data; name="myFile"; filename="test.txt"
Content-Type: text/plain

Simple file.
-----------------------------8721656041911415653955004498--


 */

  void Session::Impl::SetMultipart(Multipart&& multipart) {
    constexpr auto boundary_str = "---------------------------8721656041911415653955004498";

    req_.set(http::field::content_type, std::string("multipart/form-data; boundary=") + boundary_str);

    std::stringstream body;

    for (auto& part : multipart.parts) {
      
      body << boundary_str << CRLF;
      body << "Content-Disposition: form-data; name=" << part.name 
        << CRLF
        << CRLF;
       

      if (part.is_file) {
        body << "Content-Type: application/octet-stream" << CRLF;

        std::ifstream ifs(part.value, std::ios::in | std::ios::binary); 
        ifs.exceptions(std::ios::badbit);

        ifs.seekg(0, ifs.end);
        auto file_size = ifs.tellg(); 
        ifs.seekg(0, ifs.beg);

        std::string file_content(file_size, std::string::value_type{});
        ifs.read(const_cast<char*>(file_content.data()), file_size);
        body.write(file_content.data(), file_content.size());
      } else if (part.is_buffer) {
        body << "Content-Type: application/octet-stream" << CRLF;
        body.write(part.data, part.datalen);
      } else {
        body << part.value;
      }

      body << boundary_str << CRLF;
    }

    req.body = body;
  }


  void Session::Impl::SetRedirect(const bool& ) {
    //TODO: implement following redirects
  }
  void Session::Impl::SetMaxRedirects(const MaxRedirects& ) {
    //TODO: implement max redirect
  }

  void Session::Impl::SetCookies(const Cookies& cookies, bool delete_them) {
    for (auto cookie : cookies.all()) {
      auto cookie_string = 
        xxhr::util::urlEncode(cookie.first)
        + "=" + 
        xxhr::util::urlEncode(cookie.second);

      req_.set(http::field::set_cookie, cookie_string);
    }

  }

  void Session::Impl::SetBody(Body&& body) { req_.body = body; }
  void Session::Impl::SetBody(const Body& body) { req_.body  = body; }

  void Session::Impl::QUERY(const std::string& method) {
    req_.method = method;

    http_session->run(host.data(), port.data(), target.data(), 11);
  }

  void Session::Impl::DELETE()  { this->QUERY("DELETE"); }
  void Session::Impl::GET()     { this->QUERY("GET"); }
  void Session::Impl::HEAD()    { this->QUERY("HEAD"); }
  void Session::Impl::OPTIONS() { this->QUERY("OPTIONS"); }
  void Session::Impl::PATCH()   { this->QUERY("PATCH"); }
  void Session::Impl::POST()    { this->QUERY("POST"); }
  void Session::Impl::PUT()     { this->QUERY("PUT"); }




  // Forward to above pimpl

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

#endif

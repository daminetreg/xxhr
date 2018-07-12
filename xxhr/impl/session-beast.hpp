#ifndef XXHR_DETAIL_SESSION_BEAST_HPP
#define XXHR_DETAIL_SESSION_BEAST_HPP

#include <functional>
#include <fstream>
#include <regex>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <utility>

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


namespace xxhr {

  using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
  namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
  namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

  using plain_or_tls = std::variant<std::monostate, tcp::socket, ssl::stream<tcp::socket>>;

  

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

    void QUERY(http::verb method);

    void DELETE();
    void GET();
    void HEAD();
    void OPTIONS();
    void PATCH();
    void POST();
    void PUT();

    private:
    util::url_parts url_parts_;
    std::string url_;
    Parameters parameters_;
    http::request<http::string_body> req_;
    std::chrono::milliseconds timeout_ = std::chrono::milliseconds{0};

    std::function<void(Response&&)> on_response;

    boost::asio::io_context ioc;
    boost::asio::steady_timer timeouter{ioc};
    ssl::context ctx{ssl::context::sslv23_client};
    tcp::resolver resolver_{ioc};
    plain_or_tls stream_; 
    boost::beast::flat_buffer buffer_; // (Must persist between reads)
    http::response<http::string_body> res_;

    bool is_tls_stream() {
      return std::holds_alternative<ssl::stream<tcp::socket>>(stream_);
    }

    ssl::stream<tcp::socket>& tls_stream() {
      return std::get<ssl::stream<tcp::socket>>(stream_);
    }

    tcp::socket& plain_stream() {
      return std::get<tcp::socket>(stream_);
    }

    void fail(boost::system::error_code ec, xxhr::ErrorCode xxhr_ec) { 
      //TODO: if (trace)
      std::cerr << ec << ": " << ec.message() << " distilled into : " << uint32_t(xxhr_ec) << "\n";

      on_response(xxhr::Response(
        0, // 0 for errors which are on the layer belows http, like XmlHttpRequest.
        Error{xxhr_ec},
        std::string{},
        Header{},
        url_,
        Cookies{}
      ));
    }

    // Start the asynchronous operation
    void register_request() {

      if (is_tls_stream()) {

        auto& stream = tls_stream();

        // Set SNI Hostname (many hosts need this to handshake successfully)
        // XXX: openssl specificae, abstract this shit please
        if(! SSL_set_tlsext_host_name(stream.native_handle(), url_parts_.host.data()))
        {
            boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
            std::cerr << ec.message() << "\n";
            return;
        }
      }

      // Look up the domain name
      resolver_.async_resolve(
          url_parts_.host.data(),
          url_parts_.port.data(),
          std::bind(
              &Session::Impl::on_resolve,
              shared_from_this(),
              std::placeholders::_1,
              std::placeholders::_2));
    }

    void on_resolve( boost::system::error_code ec, tcp::resolver::results_type results) {
        if(ec)
          return fail(ec, ErrorCode::HOST_RESOLUTION_FAILURE);

        // Make the connection on the IP address we get from a lookup
        tcp::socket* socket;
        if (is_tls_stream()) {
          socket = &tls_stream().next_layer();
        } else {
          socket = &plain_stream();
        }

        boost::asio::async_connect(
            *socket,
            results.begin(),
            results.end(),
            std::bind(
                &Session::Impl::on_connect,
                shared_from_this(),
                std::placeholders::_1));
    }

    void on_connect(boost::system::error_code ec) {
        if(ec)
          return fail(ec, ErrorCode::CONNECTION_FAILURE);

        if (is_tls_stream()) {
          // Perform the SSL handshake
          auto& stream = tls_stream();
          stream.async_handshake(
              ssl::stream_base::client,
              std::bind(
                  &Session::Impl::on_stream_ready,
                  shared_from_this(),
                  std::placeholders::_1));
        } else {
          // Plain HTTP
          // consider handshake was performed.
          on_stream_ready(ec);
        }
    }

    void on_stream_ready(boost::system::error_code ec) {
        if(ec)
          return fail(ec, ErrorCode::SSL_CONNECT_ERROR);

        // Send the HTTP request to the remote host
        std::visit([this](auto& stream) {
          if constexpr (std::is_same_v<std::monostate,std::decay_t<decltype(stream)>>) return;
          else {
            http::async_write(stream, req_,
                std::bind(
                    &Session::Impl::on_write,
                    shared_from_this(),
                    std::placeholders::_1,
                    std::placeholders::_2));
          }
        }, stream_);
    }

    void on_write( boost::system::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        if(ec)
          return fail(ec, ErrorCode::NETWORK_SEND_FAILURE);
        
        // Receive the HTTP response
        std::visit([this](auto& stream) {
          if constexpr (std::is_same_v<std::monostate,std::decay_t<decltype(stream)>>) return;
          else {
            http::async_read(stream, buffer_, res_,
                std::bind(
                    &Session::Impl::on_read,
                    shared_from_this(),
                    std::placeholders::_1,
                    std::placeholders::_2));
          }
        }, stream_);
    }

    void on_read( boost::system::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);

        timeouter.cancel();

        if(ec)
          return fail(ec, ErrorCode::NETWORK_RECEIVE_ERROR);

        // Write the message to standard out
        Header response_headers;
        Cookies response_cookies;
        for (auto&& header : res_.base()) {
          if (header.name() == http::field::set_cookie) {
            response_cookies
              .parse_cookie_string(std::string(header.value()));
          } else {
            response_headers
              .insert_or_assign(
                  std::string(header.name_string()),
                  std::string(header.value()));
          }
        }

        on_response(xxhr::Response(
          res_.result_int(),
          Error{},
          res_.body(),
          response_headers,
          url_,
          response_cookies
        ));

        // Gracefully close the stream
        if (is_tls_stream()) {
          auto& stream = tls_stream();
          stream.async_shutdown(
            std::bind(
              &Session::Impl::on_shutdown,
              shared_from_this(),
              std::placeholders::_1));
        } else {
          on_shutdown(ec);
        }
    }

    void on_shutdown(boost::system::error_code ec) {
      if(ec == boost::asio::error::eof) {
          // Rationale:
          // http://stackoverflow.com/questions/25587403/boost-asio-ssl-async-shutdown-always-finishes-with-an-error
          ec.assign(0, ec.category());
      }
      if(ec)
          return fail(ec, ErrorCode::GENERIC_SSL_ERROR);

      // If we get here then the connection is closed gracefully
    }

    void on_timeout(const boost::system::error_code& ec) {
      if (ec != boost::asio::error::operation_aborted) {
        
        ioc.stop();

        tcp::socket* socket;
        if (is_tls_stream()) {
          socket = &tls_stream().next_layer();
        } else {
          socket = &plain_stream();
        }

        boost::system::error_code ec_dontthrow;
        socket->cancel(ec_dontthrow);
        socket->close(ec_dontthrow);

        fail(ec, ErrorCode::TIMEDOUT);
      }
    }


  };

  void Session::Impl::SetUrl(const Url& url) {
    url_ = url; 
    url_parts_ = util::parse_url(url); 
  }
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
    std::stringstream ss; ss << "Basic " << util::encode64(auth.GetAuthString());
    req_.set(http::field::authorization, ss.str());
  }

  void Session::Impl::SetDigest(const Digest& auth) {
    //TODO: Replace BASIC auth here by Digest based authentication (MD5 summing the info)
    namespace http = boost::beast::http;
    std::stringstream ss; ss << "Basic " << util::encode64(auth.GetAuthString());
    req_.set(http::field::authorization, ss.str());
  }

  void Session::Impl::SetPayload(Payload&& payload) {
    req_.set(http::field::content_type, "application/x-www-form-urlencoded");
    req_.body() = payload.content;
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
        body.write(reinterpret_cast<const char*>(part.data), part.datalen);
      } else {
        body << part.value;
      }

      body << boundary_str << CRLF;
    }

    req_.body() = body.str();
    req_.set(http::field::content_length, req_.body().size());
  }

  void Session::Impl::SetMultipart(const Multipart& multipart) {
    SetMultipart(std::move(const_cast<Multipart&>(multipart)));
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

  void Session::Impl::SetBody(Body&& body) { req_.body() = body; }
  void Session::Impl::SetBody(const Body& body) { req_.body()  = body; }

  void Session::Impl::QUERY(http::verb method) {
    req_.method(method);

    // Set up an HTTP GET request message
    req_.version(11);
    std::stringstream target; target << url_parts_.path;
    if ( !url_parts_.parameters.empty() || !parameters_.content.empty() ) {
      target << "?" << url_parts_.parameters << parameters_.content;
    }
    req_.target(target.str());
    req_.set(http::field::host, url_parts_.host);
    req_.set(http::field::user_agent, "xxhr/v0.0.1"); //TODO: add a way to override from user and make a version macro

    if (url_parts_.https()) {
      stream_.emplace<ssl::stream<tcp::socket>>(ioc, ctx);
    } else {
      stream_.emplace<tcp::socket>(ioc);
    }

    register_request();

    if (timeout_ != std::chrono::milliseconds(0)) {
      timeouter.expires_after(timeout_);
      timeouter.async_wait(std::bind(&Session::Impl::on_timeout, shared_from_this(), std::placeholders::_1)); 
    }

    ioc.run();
  }

  void Session::Impl::DELETE()  { this->QUERY(http::verb::delete_); }
  void Session::Impl::GET()     { this->QUERY(http::verb::get); }
  void Session::Impl::HEAD()    { this->QUERY(http::verb::head); }
  void Session::Impl::OPTIONS() { this->QUERY(http::verb::options); }
  void Session::Impl::PATCH()   { this->QUERY(http::verb::patch); }
  void Session::Impl::POST()    { this->QUERY(http::verb::post); }
  void Session::Impl::PUT()     { this->QUERY(http::verb::put); }




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

// Copyright 2017-present nxxm.github.io
// Copyright 2017-present Damien Buhl (alias daminetreg)
// Copyright 2017-present Ibukun Oladipo tormuto (urlEncode, urlDecode on stack-overflow)
#ifndef XXHR_UTIL_H
#define XXHR_UTIL_H

#include <cctype>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include <boost/xpressive/xpressive.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include "xxhrtypes.hpp"


namespace xxhr {
namespace util {

  class named_ofstream : public std::ofstream {
  public:
    named_ofstream() = default;
    named_ofstream(std::string filename) 
      : _filename{filename}
      , std::ofstream(filename, std::ios::out | std::ios::trunc | std::ios::binary) {}
    
    /// \brief get the filename of this stream
    const std::string& get_filename() { return _filename; }
  private:
    std::string _filename;
  };
 
  inline Header parseHeader(const std::string& headers);
  inline size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data);
  inline size_t writeFileFunction(char *ptr, size_t size, size_t nmemb, void* ofstream_ptr);
  inline std::vector<std::string> split(const std::string& to_split, char delimiter);

  /**
   * \brief Simple URL Encoding function useful for uses in different contexts.
   */
  inline std::string urlEncode(const std::string& response);
  inline std::string decode64(const std::string &val);
  inline std::string encode64(const std::string &val);

  /**
   * \brief Parts of a parsed url.
   */
  struct url_parts {

    //!
    std::string protocol;

    //!
    std::string host;

    //!
    std::string port;

    //!
    std::string path;

    //!
    std::string parameters;

    //!
    std::string fragment;

    //! Wether the url requires TLS
    bool https() const {
      return protocol == "https";
    }
  };

  //! Parses an URL into xxhr::util::url_parts 
  inline url_parts parse_url(const std::string &url);

} // namespace util
} // namespace xxhr


/*
 * Implementation
 */

namespace xxhr { 
namespace util {

  inline Header parseHeader(const std::string& headers) {
    Header header;
    std::vector<std::string> lines;
    std::istringstream stream(headers);
    {
        std::string line;
        while (std::getline(stream, line, '\n')) {
            lines.push_back(line);
        }
    }

    for (auto& line : lines) {
        if (line.substr(0, 5) == "HTTP/") {
            header.clear();
        }

        if (line.length() > 0) {
            auto found = line.find(":");
            if (found != std::string::npos) {
                auto value = line.substr(found + 2, line.length() - 1);
                if (value.back() == '\r') {
                    value = value.substr(0, value.length() - 1);
                }
                header[line.substr(0, found)] = value;
            }
        }
    }

    return header;
  }

  std::vector<std::string> split(const std::string& to_split, char delimiter) {
    std::vector<std::string> tokens;

    std::stringstream stream(to_split);
    std::string item;
    while (std::getline(stream, item, delimiter)) {
        tokens.push_back(item);
    }

    return tokens;
  }

  inline size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*) ptr, size * nmemb);
    return size * nmemb;
  }

  inline size_t writeFileFunction(char *ptr, size_t size, size_t nmemb, void* ofstream_ptr)
  {
    named_ofstream *ofstream = static_cast<named_ofstream *>(ofstream_ptr);

    // calc the total size
    size_t nbytes = size * nmemb;

    if (!ofstream->is_open()) {
      auto ec = std::make_error_condition(std::errc::io_error);
      throw std::runtime_error("Writing File "s + ofstream->get_filename() + " error "s + ec.message());
    };


    ofstream->write(ptr, nbytes);
    
    if (ofstream->bad()) {
      auto ec = std::make_error_condition(std::errc::io_error);
      throw std::runtime_error("Writing File "s + ofstream->get_filename() + " error "s + ec.message());
    }

    return nbytes;
  }


  inline std::string urlEncode(const std::string& value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (auto i = value.cbegin(), n = value.cend(); i != n; ++i) {
        std::string::value_type c = (*i);
        // Keep alphanumeric and other accepted characters intact
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }
        // Any other characters are percent-encoded
        escaped << '%' << std::setw(2) << std::int32_t((unsigned char) c);
    }

    return escaped.str();
  }

  inline std::string urlDecode(const std::string& str){
    std::string ret;
    char ch;
    int i, ii, len = str.length();

    for (i=0; i < len; i++){
        if(str[i] != '%'){
            if(str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }else{
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
  }

  inline std::string decode64(const std::string &val) {
      using namespace boost::archive::iterators;
      using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
      // See https://svn.boost.org/trac10/ticket/5629#comment:9
      // Boost binary_from_base64 transforms '=' into '\0', they need to be removed to support binary data
      auto padding_count = std::count(val.end() - std::min(std::size_t{2}, val.size()), val.end() , '=');
      return std::string(It(std::begin(val)), It(std::end(val) - padding_count));
  }


  inline std::string encode64(const std::string &val) {
      using namespace boost::archive::iterators;
      using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
      auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
      return tmp.append((3 - val.size() % 3) % 3, '=');
  }


  const boost::xpressive::sregex url_regex = 
    boost::xpressive::sregex::compile("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");

  inline url_parts parse_url(const std::string &url) {
    url_parts ret;
    
    // Way too slow on MSVC (55 seconds in avg for an URI, less than one ms on Gcc / clang )
    //std::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
    //std::smatch what;
    //if (regex_match(url, what, ex)) {
    using namespace boost::xpressive;
    smatch what;
    if( regex_match( url, what, url_regex ) ) {

        ret.protocol = what[1];
        ret.host   = what[2];
        ret.port     = what[3];

        if (ret.port.empty()) {
          ret.port = (ret.protocol == "https") ? "443" : "80";
        }

        ret.path     = what[4];
        ret.parameters    = what[5];
        ret.fragment = what[6];
    }

    return ret;
  }


} // namespace util
} // namespace xxhr 

#endif

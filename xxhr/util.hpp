#ifndef XXHR_UTIL_H
#define XXHR_UTIL_H

#include <cctype>
#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

#include "xxhrtypes.hpp"


namespace xxhr {
namespace util {
 
  inline Header parseHeader(const std::string& headers);
  inline size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data);
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

  inline std::string decode64(const std::string &val) {
      using namespace boost::archive::iterators;
      using It = transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
      return boost::algorithm::trim_right_copy_if(std::string(It(std::begin(val)), It(std::end(val))), [](char c) {
          return c == '\0';
      });
  }

  inline std::string encode64(const std::string &val) {
      using namespace boost::archive::iterators;
      using It = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
      auto tmp = std::string(It(std::begin(val)), It(std::end(val)));
      return tmp.append((3 - val.size() % 3) % 3, '=');
  }

  inline url_parts parse_url(const std::string &url) {
    url_parts ret;
    std::regex ex("(http|https)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
    std::cmatch what;
    if (regex_match(url.c_str(), what, ex)) {
        ret.protocol = std::string(what[1].first, what[1].second);
        ret.host   = std::string(what[2].first, what[2].second);
        ret.port     = std::string(what[3].first, what[3].second);

        if (ret.port.empty()) {
          ret.port = (ret.protocol == "https") ? "443" : "80";
        }

        ret.path     = std::string(what[4].first, what[4].second);
        ret.parameters    = std::string(what[5].first, what[5].second);
        ret.fragment = std::string(what[6].first, what[6].second);
    }

    return ret;
  }


} // namespace util
} // namespace xxhr 

#endif

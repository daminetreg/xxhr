#ifndef XXHR_COOKIES_H
#define XXHR_COOKIES_H

#include <initializer_list>
#include <map>
#include <sstream>
#include <string>
#include <regex>
#include <utility>
#include <functional>

#include <boost/algorithm/string/trim_all.hpp>

#include "util.hpp"

#include <iostream>

namespace xxhr {

class Cookies {
  public:
  Cookies() {}
  Cookies(const std::initializer_list<std::pair<const std::string, std::string>>& pairs) 
    : map_{pairs} {};
  Cookies(const std::map<std::string, std::string>& map) : map_{map} {}

  std::string& operator[](const std::string& key) { return map_[key]; }

  std::string GetEncoded() const {
    std::stringstream stream;
    for (const auto& item : map_) {
        stream << xxhr::util::urlEncode(item.first) << "=";
        // special case version 1 cookies, which can be distinguished by
        // beginning and trailing quotes
        if (!item.second.empty() && item.second.front() == '"' && item.second.back() == '"') {
            stream << item.second;
        } else {
            stream << xxhr::util::urlEncode(item.second);
        }
        stream << "; ";
    }
    return stream.str();
  }

  inline void parse_cookie_string(const std::string& cookie_str) {
    using namespace boost::algorithm;

    std::regex cookies_rx(";");
    std::vector<std::string> cookies(
      std::sregex_token_iterator(
        cookie_str.begin(), cookie_str.end(), cookies_rx, -1),
      std::sregex_token_iterator()
    );

    for (auto& cookie : cookies) {
      std::regex rx_cookie("([^=]+)=([^;]+)");
      std::smatch match;
      std::regex_match(cookie, match, rx_cookie);
      map_.insert(std::make_pair(trim_all_copy(match[1].str()), match[2].str()));
    }
  }

  private:
  std::map<std::string, std::string> map_;

  public:
  decltype(map_) all() const { return map_; }
};


} // namespace xxhr

#endif

#ifndef XXHR_XXHR_TYPES_H
#define XXHR_XXHR_TYPES_H

#include <algorithm>
#include <map>
#include <string>

namespace xxhr {

  constexpr const auto CRLF = "\r\n";


  struct CaseInsensitiveCompare {
      bool operator()(const std::string& a, const std::string& b) const noexcept {
        return std::lexicographical_compare(
                a.begin(), a.end(), b.begin(), b.end(),
                [](unsigned char ac, unsigned char bc) { return std::tolower(ac) < std::tolower(bc); });
      }
  };

  using Header = std::map<std::string, std::string, CaseInsensitiveCompare>;
  using Url = std::string;

} // namespace xxhr

#endif

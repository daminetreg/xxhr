#ifndef XXHR_PROXIES_HPP
#define XXHR_PROXIES_HPP

#include <initializer_list>
#include <map>
#include <string>

namespace xxhr {
  class Proxies {
    public:
    Proxies() = default;
    Proxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts)
      : hosts_{hosts} {}

    bool has(const std::string& protocol) const { return hosts_.count(protocol) > 0; }
    const std::string& operator[](const std::string& protocol) { return hosts_[protocol]; }

    private:
    std::map<std::string, std::string> hosts_;
  };
}

#endif

#ifndef XXHR_PAYLOAD_H
#define XXHR_PAYLOAD_H

#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>

#include "util.hpp"

#include "defines.hpp"

namespace xxhr {

  //TODO: refactor to std::pair

struct Pair {
    template <typename KeyType, typename ValueType,
              typename std::enable_if<!std::is_integral<ValueType>::value, bool>::type = true>
    Pair(KeyType&& p_key, ValueType&& p_value)
            : key{XXHR_FWD(p_key)}, value{XXHR_FWD(p_value)} {}
    template <typename KeyType>
    Pair(KeyType&& p_key, const std::int32_t& p_value)
            : key{XXHR_FWD(p_key)}, value{std::to_string(p_value)} {}

    std::string key;
    std::string value;
};

/**
 * \brief Form encoded payload for POST or PUT request. Simply made of key values pairs.
 */
class Payload {
  public:
    template <class It>
    Payload(const It begin, const It end) {
        for (It pair = begin; pair != end; ++pair) {
            AddPair(*pair);
        }
    }

    Payload(const std::initializer_list<Pair>& pairs) : Payload(begin(pairs), end(pairs)) {}

    void AddPair(const Pair& pair) {
      if (!content.empty()) {
          content += "&";
      }
      auto escaped = xxhr::util::urlEncode(pair.value);
      content += pair.key + "=" + escaped;
    }

    std::string content;
};

} // namespace xxhr

#endif

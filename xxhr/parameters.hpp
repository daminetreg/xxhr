#ifndef XXHR_PARAMETERS_H
#define XXHR_PARAMETERS_H

#include <initializer_list>
#include <memory>
#include <string>

#include "util.hpp"

#include "defines.hpp"

namespace xxhr {

struct Parameter {
    template <typename KeyType, typename ValueType>
    Parameter(KeyType&& key, ValueType&& value)
            : key{XXHR_FWD(key)}, value{XXHR_FWD(value)} {}

    std::string key;
    std::string value;
};

/**
 * \brief Allows passing an initializer list of named HTTP Parameters that will be URL Encoded within the request URI ( *i.e.* In the form your/url?param=value).
 *
 * For example querying google about this library can be done with :
 *
 * \snippet examples/parameters.cpp Parameters-snippet
 *
 */
class Parameters {
  public:
    Parameters() = default;
    Parameters(const std::initializer_list<Parameter>& parameters) {
      for (const auto& parameter : parameters) {
          AddParameter(parameter);
      }
    }

    void AddParameter(const Parameter& parameter) {
      if (!content.empty()) {
          content += "&";
      }

      auto escapedKey = xxhr::util::urlEncode(parameter.key);
      if (parameter.value.empty()) {
          content += escapedKey;
      } else {
          auto escapedValue = xxhr::util::urlEncode(parameter.value);
          content += escapedKey + "=" + escapedValue;
      }
    }


    std::string content;
};

} // namespace xxhr

#endif

#ifndef XXHR_MAX_REDIRECTS_H
#define XXHR_MAX_REDIRECTS_H

#include <cstdint>

namespace xxhr {

class MaxRedirects {
  public:
    explicit MaxRedirects(const std::int32_t number_of_redirects)
            : number_of_redirects(number_of_redirects) {}

    std::int32_t number_of_redirects;
};

} // namespace xxhr

#endif

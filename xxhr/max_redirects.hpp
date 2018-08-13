#ifndef XXHR_MAX_REDIRECTS_H
#define XXHR_MAX_REDIRECTS_H

#include <cstdint>

namespace xxhr {

/**
 * \brief HTTP servers can reply with http redirection header, to avoid infinite redirection client application can define a maximum redirect count.
 *        By default when not supplied std::numeric_limits<std::int32_t>::max() is used as maximum redirection.
 */
class MaxRedirects {
  public:
    //!
    explicit MaxRedirects(const std::int32_t number_of_redirects)
            : number_of_redirects(number_of_redirects) {}

    std::int32_t number_of_redirects;
};

} // namespace xxhr

#endif

#ifndef XXHR_ERROR_H
#define XXHR_ERROR_H

#include <cstdint>
#include <string>
#include <sstream>

#include <enum.h>

#include "xxhrtypes.hpp"
#include "defines.hpp"

namespace xxhr {


//TODO: refactor into system.error_code own category.

BETTER_ENUM(ErrorCode, unsigned int,
//! [ErrorCodes]
    OK = 0,
    TIMEDOUT,
    CONNECTION_FAILURE,
    EMPTY_RESPONSE,
    HOST_RESOLUTION_FAILURE,
    INTERNAL_ERROR,
    INVALID_URL_FORMAT,
    NETWORK_RECEIVE_ERROR,
    NETWORK_SEND_FAILURE,
    PROXY_RESOLUTION_FAILURE,
    SSL_CONNECT_ERROR,
    SSL_LOCAL_CERTIFICATE_ERROR,
    SSL_REMOTE_CERTIFICATE_ERROR,
    SSL_CACERT_ERROR,
    GENERIC_SSL_ERROR,
    UNSUPPORTED_PROTOCOL,
    REQUEST_CANCELLED,
    TOO_MANY_REDIRECTS, 
    UNKNOWN_ERROR = 1000
//! [ErrorCodes]
);

/** 
 * \brief Represents Errors happening at a lower layer than HTTP.
 * 
 * Possible error codes are : 
 * \snippet this ErrorCodes
 */
class Error {
  public:
    Error() : code{ErrorCode::OK} {}
    Error(ErrorCode code) : code{code} {}

    explicit operator bool() const {
        return code != +ErrorCode::OK;
    }

    ErrorCode code;

    //! Converts Error to human readable std::string
    operator std::string() const {
      std::stringstream ss;
      ss << "error:" << static_cast<unsigned int>(this->code) << " " << this->code._to_string();
      return ss.str();
    }
};

inline std::ostream& operator<<(std::ostream& os, const Error& err) {
  os << std::string(err);
  return os;
}

} // namespace xxhr

#endif

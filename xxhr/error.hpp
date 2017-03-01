#ifndef XXHR_ERROR_H
#define XXHR_ERROR_H

#include <cstdint>
#include <string>

#include "xxhrtypes.hpp"
#include "defines.hpp"

namespace xxhr {

enum class ErrorCode {
    OK = 0,
    CONNECTION_FAILURE,
    EMPTY_RESPONSE,
    HOST_RESOLUTION_FAILURE,
    INTERNAL_ERROR,
    INVALID_URL_FORMAT,
    NETWORK_RECEIVE_ERROR,
    NETWORK_SEND_FAILURE,
    OPERATION_TIMEDOUT,
    PROXY_RESOLUTION_FAILURE,
    SSL_CONNECT_ERROR,
    SSL_LOCAL_CERTIFICATE_ERROR,
    SSL_REMOTE_CERTIFICATE_ERROR,
    SSL_CACERT_ERROR,
    GENERIC_SSL_ERROR,
    UNSUPPORTED_PROTOCOL,
    UNKNOWN_ERROR = 1000,
};

class Error {
  public:
    Error() : code{ErrorCode::OK} {}
    Error(ErrorCode code) : code{code} {}

    explicit operator bool() const {
        return code != ErrorCode::OK;
    }

    ErrorCode code;
};

} // namespace xxhr

#endif

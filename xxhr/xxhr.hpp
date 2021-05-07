#ifndef XXHR_XXHR_H
#define XXHR_XXHR_H

#define XXHR_VERSION "v1.0.1"

#if defined(_WIN32) and defined(__MINGW32__)
# undef X509_NAME
 # undef X509_CERT_PAIR
 # undef X509_EXTENSIONS
 #undef OCSP_REQUEST
 #undef OCSP_RESPONSE
#endif

#include "api.hpp"
#include "auth.hpp"
#include "xxhrtypes.hpp"
#include "response.hpp"
#include "session.hpp"

#endif

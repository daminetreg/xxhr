#ifndef XXHR_API_H
#define XXHR_API_H

#include <functional>
#include <string>

#include "auth.hpp"
#include "xxhrtypes.hpp"
#include "defines.hpp"
#include "digest.hpp"
#include "multipart.hpp"
#include "payload.hpp"
#include "response.hpp"
#include "session.hpp"
#include "handler.hpp"

/**
 * \brief main library namespace
 */
namespace xxhr {

  namespace priv {

    template <typename T>
    void set_option(Session& session, T&& t) {
        session.SetOption(XXHR_FWD(t));
    }

    template <typename T, typename... Ts>
    void set_option(Session& session, T&& t, Ts&&... ts) {
        set_option(session, XXHR_FWD(t));
        set_option(session, XXHR_FWD(ts)...);
    }

  } // namespace priv

/**
 * \brief Performs an HTTP GET Request.
 */
template <typename... Ts>
void GET(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.GET(); }

template <typename... Ts>
void POST(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.POST(); }

template <typename... Ts>
void PUT(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.PUT(); }

template <typename... Ts>
void HEAD(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.HEAD(); }

template <typename... Ts>
void DELETE(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.DELETE(); }

template <typename... Ts>
void OPTIONS(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.OPTIONS(); }

template <typename... Ts>
void PATCH(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.PATCH(); }

} // namespace xxhr

#endif

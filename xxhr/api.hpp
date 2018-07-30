#ifndef XXHR_API_H
#define XXHR_API_H

#include <functional>
#include <string>

#include "auth.hpp"
#include "xxhrtypes.hpp"
#include "defines.hpp"
#include "digest.hpp"
#include "multipart.hpp"
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
 * \brief HTTP GET Request.
 */
template <typename... Ts>
void GET(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.GET(); }

/**
 * \brief HTTP POST Request.
 */
template <typename... Ts>
void POST(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.POST(); }

/**
 * \brief HTTP PUT Request.
 */
template <typename... Ts>
void PUT(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.PUT(); }

/**
 * \brief HTTP HEAD Request.
 */
template <typename... Ts>
void HEAD(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.HEAD(); }

/**
 * \brief HTTP DELETE Request.
 */
template <typename... Ts>
void DELETE(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.DELETE(); }

/**
 * \brief HTTP OPTIONS Request.
 */
template <typename... Ts>
void OPTIONS(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.OPTIONS(); }

/**
 * \brief HTTP PATCH Request.
 */
template <typename... Ts>
void PATCH(Ts&&... ts) { Session session; priv::set_option(session, XXHR_FWD(ts)...); session.PATCH(); }

} // namespace xxhr

#endif

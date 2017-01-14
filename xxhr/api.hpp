#ifndef XXHR_API_H
#define XXHR_API_H

#include <functional>
#include <future>
#include <string>

#include "auth.hpp"
#include "xxhrtypes.hpp"
#include "defines.hpp"
#include "digest.hpp"
#include "multipart.hpp"
#include "payload.hpp"
#include "response.hpp"
#include "session.hpp"

namespace xxhr {

using AsyncResponse = std::future<Response>;

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

// GET methods
template <typename... Ts>
Response GET(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.GET();
}

// GET async methods
template <typename... Ts>
AsyncResponse GETAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return GET(std::move(ts)...); },
                      std::move(ts)...);
}

// GET callback methods
template <typename Then, typename... Ts>
auto GETCallback(Then then, Ts... ts) -> std::future<decltype(then(GET(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(GET(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

// POST methods
template <typename... Ts>
Response POST(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.POST();
}

// POST async methods
template <typename... Ts>
AsyncResponse PostAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return POST(std::move(ts)...); },
                      std::move(ts)...);
}

// POST callback methods
template <typename Then, typename... Ts>
auto PostCallback(Then then, Ts... ts) -> std::future<decltype(then(POST(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(POST(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

// PUT methods
template <typename... Ts>
Response PUT(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.PUT();
}

// PUT async methods
template <typename... Ts>
AsyncResponse PUTAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return PUT(std::move(ts)...); },
                      std::move(ts)...);
}

// PUT callback methods
template <typename Then, typename... Ts>
auto PUTCallback(Then then, Ts... ts) -> std::future<decltype(then(PUT(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(PUT(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

// HEAD methods
template <typename... Ts>
Response HEAD(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.HEAD();
}

// HEAD async methods
template <typename... Ts>
AsyncResponse HeadAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return HEAD(std::move(ts)...); },
                      std::move(ts)...);
}

// HEAD callback methods
template <typename Then, typename... Ts>
auto HeadCallback(Then then, Ts... ts) -> std::future<decltype(then(HEAD(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(HEAD(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

// DELETE methods
template <typename... Ts>
Response DELETE(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.DELETE();
}

// DELETE async methods
template <typename... Ts>
AsyncResponse DeleteAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return DELETE(std::move(ts)...); },
                      std::move(ts)...);
}

// DELETE callback methods
template <typename Then, typename... Ts>
auto DeleteCallback(Then then, Ts... ts) -> std::future<decltype(then(DELETE(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(DELETE(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

// OPTIONS methods
template <typename... Ts>
Response OPTIONS(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.OPTIONS();
}

// OPTIONS async methods
template <typename... Ts>
AsyncResponse OptionsAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return OPTIONS(std::move(ts)...); },
                      std::move(ts)...);
}

// OPTIONS callback methods
template <typename Then, typename... Ts>
auto OptionsCallback(Then then, Ts... ts)
        -> std::future<decltype(then(OPTIONS(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(OPTIONS(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

// PATCH methods
template <typename... Ts>
Response PATCH(Ts&&... ts) {
    Session session;
    priv::set_option(session, XXHR_FWD(ts)...);
    return session.PATCH();
}

// PATCH async methods
template <typename... Ts>
AsyncResponse PatchAsync(Ts... ts) {
    return std::async(std::launch::async, [](Ts... ts) { return PATCH(std::move(ts)...); },
                      std::move(ts)...);
}

// PATCH callback methods
template <typename Then, typename... Ts>
auto PatchCallback(Then then, Ts... ts) -> std::future<decltype(then(PATCH(std::move(ts)...)))> {
    return std::async(std::launch::async, [](Then then, Ts... ts) {
        return then(PATCH(std::move(ts)...));
    }, std::move(then), std::move(ts)...);
}

} // namespace xxhr

#endif

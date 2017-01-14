#ifndef XXHR_DEFINES_H
#define XXHR_DEFINES_H

#include <utility>

#define XXHR_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#endif

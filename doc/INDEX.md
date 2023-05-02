xxhr : c++ http requests                         {#mainpage}
========================
A simple, yet modern, http client library, all dependencies included !

## API: asynchronously intuitive
\include examples/introductory.hpp

## GETting Started

* `#include <xxhr/xxhr.hpp>` with [`.nxxm/deps`](https://nxxm.github.io) : 

```
{
  "nxxm/xxhr" : { "@" : "v1.0.1" }
}
```

* [GET Started](@ref getting-started-cpp).

## Platforms
![pipeline status](https://git.lecbna.org/daminetreg/xxhr/badges/master/pipeline.svg)

- WebAssembly
- macOS
- Linux
- Windows

## Motivation
http[s] queries made async & easy for C++ developer.

The API is exclusively async
  - to target all modern platforms and runtimes ( *e.g.* Mobile OS, Web Browser, Windows 10... ).
  - to build responsive UIs. 

Pure & modern, this header-only C++ library is available for WebAssembly and on all C++ compatible platforms.

## Key Principles

  * Just headers files.
  * Dependencies shipped: [`nxxm .`](https://nxxm.github.io/).
  * Async API.
  * [Extremely cross-platform](doc/supported_platforms.md) : Native or WebAssembly.
  * Performant, easy to setup \& learn !



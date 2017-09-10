xxhr : c++ http requests                         {#mainpage}
========================
Why on earth it has to be so difficult to make http queries ?
There isn't anything simpler than that, that's only some characters sent over a connection.
We already had so many good header-only libraries in C++, now there is one for http client.

  * Just headers files
  * No binary dependencies
  * Dependencies shipped: [we auto-satisfy them](https://github.com/header-only/inglued).
  * [Extremely cross-platform](doc/supported_platforms.md)

A simple, yet modern, http client library, all dependencies included !

## An intuitive API
\include examples/introductory.cpp

## Getting Started

**Just include us** `#include <xxhr/xxhr.hpp>` : [Details on how to install](INSTALL.md).

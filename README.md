![xxhr logo](./doc/logo.ai.png)
# xxhr : c++ http requests
Intuitive c++ http client library.

## How to use it ?
  * Just include it : `#include <xxhr/xxhr.hpp>`
  * Or if you are a CMake freak as I am : `find_package(xxhr) target_link_library( your_program xxhr::xxhr)`

### Cross platformness
The underlying backend for the implementation for all native platforms will be Boost.Beast, while the other backend for browser environment is **XmlHttpRequest**.

## Roadmap

### End of April 2016
  - Implement native (windows, linux, macos) http client backend with Boost.Beast.

## License
xxhr is a rewrite of the [cpr](https://github.com/whoshuu/cpr) project from Huu Nguyen ( [https://github.com/whoshuu](@whoshuu) ) with a different philosophy :
  - Easy to consume: no binaries dependencies, only headers (no curl).
  - Asynchronous by essence

[Boost Sofware License](./LICENSE), please just reproduce copyrights : 

```
Copyright (c) 2017 Damien Buhl alias daminetreg
Copyright (c) 2015 - 2017 Huu Nguyen
```

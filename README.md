# xxhr : C++ http requests
xxhr is a fork of the [cpr](https://github.com/whoshuu/cpr) project from Huu Nguyen ( [https://github.com/whoshuu](@whoshuu) ) with a modern C++spiritual difference : it doesn't depend on curl & it is header only.

The API defined by ( [https://github.com/whoshuu](@whoshuu) ) is inspired from the Python Requests project, and this fork keeps this spirit of having a simple API for being a C++14 friendly http client api based on http implementation which is not dependent on anything else than **std::** or **boost::**.

## How to use it ?

  * Just include it : `#include <xxhr/api.hpp>`
  * Or if you are a CMake freak as I am : `find_package(xxhr) target_link_library( your_program xxhr::xxhr)`

## Cross platform
The underlying backend for the implementation for all native platforms is Boost.Beast, while the other backend is XmlHttpRequest.

## License
[MIT License](./LICENSE), please reproduce copyrights : 

```
Copyright (c) 2017 Damien Buhl alias daminetreg
Copyright (c) 2015 - 2017 Huu Nguyen
```

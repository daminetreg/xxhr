
#  Official project now in https://github.com/nxxm/xxhr


![xxhr logo](./doc/logo.png)
[![pipeline status](https://git.lecbna.org/daminetreg/xxhr/badges/master/pipeline.svg)](https://git.lecbna.org/daminetreg/xxhr/commits/master)

# xxhr : c++ http requests
Intuitive c++ http client library for WASM and all other platforms.

* `#include <xxhr/xxhr.hpp>` with [`.nxxm/deps`](https://nxxm.github.io) : 

```
{
  "daminetreg/xxhr" : { "@" : "v1.0.0" }
}
```

* [GETing Started Tutorial](https://daminetreg.github.io/xxhr/html/getting-started-cpp.html)
* [Reference Documentation](https://daminetreg.github.io/xxhr/html/namespacexxhr.html)
* [Examples](https://daminetreg.github.io/xxhr/html/pages.html)

## License
xxhr is a rewrite of the [cpr](https://github.com/whoshuu/cpr) project from Huu Nguyen ( [https://github.com/whoshuu](@whoshuu) ) with a different philosophy :
  - Easy to consume: no binaries dependencies, only headers (no curl).
  - Asynchronous API by essence

[MIT License](./LICENSE.md), please just reproduce copyrights : 

```
Copyright (c) 2017 - present Damien Buhl alias daminetreg
Copyright (c) 2015 - 2017 Huu Nguyen
```


# Installing cpp-js::xxhr
xxhr can be installed in three ways :

* [Copy paste in your project](#copy-paste-in-your-project)
* [Install](#install)
* [Install without dependencies](#install-without-dependencies)

## Copy paste in your project
1. Copy this folder in your project
2. Add the following to your include path :

### GCC / Clang / MSVC
```
-I xxhr/ \
-I xxhr/deps/boostorg/algorithm/include/ \
-I xxhr/deps/boostorg/align/include/ \
-I xxhr/deps/boostorg/array/include/ \
-I xxhr/deps/boostorg/asio/include/ \
-I xxhr/deps/boostorg/assert/include/ \
-I xxhr/deps/boostorg/atomic/include/ \
-I xxhr/deps/boostorg/bind/include/ \
-I xxhr/deps/boostorg/chrono/include/ \
-I xxhr/deps/boostorg/concept_check/include/ \
-I xxhr/deps/boostorg/config/include/ \
-I xxhr/deps/boostorg/container/include/ \
-I xxhr/deps/boostorg/context/include/ \
-I xxhr/deps/boostorg/core/include/ \
-I xxhr/deps/boostorg/coroutine/include/ \
-I xxhr/deps/boostorg/date_time/include/ \
-I xxhr/deps/boostorg/detail/include/ \
-I xxhr/deps/boostorg/endian/include/ \
-I xxhr/deps/boostorg/exception/include/ \
-I xxhr/deps/boostorg/filesystem/include/ \
-I xxhr/deps/boostorg/foreach/include/ \
-I xxhr/deps/boostorg/function/include/ \
-I xxhr/deps/boostorg/function_types/include/ \
-I xxhr/deps/boostorg/functional/include/ \
-I xxhr/deps/boostorg/fusion/include/ \
-I xxhr/deps/boostorg/integer/include/ \
-I xxhr/deps/boostorg/intrusive/include/ \
-I xxhr/deps/boostorg/io/include/ \
-I xxhr/deps/boostorg/iostreams/include/ \
-I xxhr/deps/boostorg/iterator/include/ \
-I xxhr/deps/boostorg/lambda/include/ \
-I xxhr/deps/boostorg/lexical_cast/include/ \
-I xxhr/deps/boostorg/locale/include/ \
-I xxhr/deps/boostorg/math/include/ \
-I xxhr/deps/boostorg/move/include/ \
-I xxhr/deps/boostorg/mpl/include/ \
-I xxhr/deps/boostorg/numeric_conversion/include/ \
-I xxhr/deps/boostorg/optional/include/ \
-I xxhr/deps/boostorg/phoenix/include/ \
-I xxhr/deps/boostorg/pool/include/ \
-I xxhr/deps/boostorg/predef/include/ \
-I xxhr/deps/boostorg/preprocessor/include/ \
-I xxhr/deps/boostorg/proto/include/ \
-I xxhr/deps/boostorg/random/include/ \
-I xxhr/deps/boostorg/range/include/ \
-I xxhr/deps/boostorg/ratio/include/ \
-I xxhr/deps/boostorg/rational/include/ \
-I xxhr/deps/boostorg/regex/include/ \
-I xxhr/deps/boostorg/serialization/include/ \
-I xxhr/deps/boostorg/smart_ptr/include/ \
-I xxhr/deps/boostorg/spirit/include/ \
-I xxhr/deps/boostorg/static_assert/include/ \
-I xxhr/deps/boostorg/system/include/ \
-I xxhr/deps/boostorg/thread/include/ \
-I xxhr/deps/boostorg/throw_exception/include/ \
-I xxhr/deps/boostorg/tokenizer/include/ \
-I xxhr/deps/boostorg/tti/include/ \
-I xxhr/deps/boostorg/tuple/include/ \
-I xxhr/deps/boostorg/type_index/include/ \
-I xxhr/deps/boostorg/type_traits/include/ \
-I xxhr/deps/boostorg/typeof/include/ \
-I xxhr/deps/boostorg/utility/include/ \
-I xxhr/deps/boostorg/variant/include/ \
-I xxhr/deps/cpp-pre/type_traits/ \
-I xxhr/deps/daminetreg/js-bind/ \
-I xxhr/deps/header-only/bearssl/include/ \
```

## Install
Install the library
```sh
mkdir build/ && cd build/
cmake ..
make install
```

You can tweak destination with `-DCMAKE_INSTALL_PREFIX=/install/path` to change the default `/usr/local`.

## If you use CMake :
```cmake
find_package(xxhr REQUIRED)
add_executable(app your_code.cpp)
target_link_libraries(app cpp-js::xxhr)
```

## If you don't use CMake : 
Add to your include path : 
```
INSTALL_PREFIX/include/xxhr/deps/
INSTALL_PREFIX/include/
```

## Install without dependencies
Install the library
```sh
mkdir build/ && cd build/
cmake .. -DINGLUED=OFF
make install
```

This library was built with [`#inglued <>`](https://github.com/header-only/inglued) therefore it forwards dependencies with it, but you might prefer the use of your own deps, therefore simply disable them via `INGLUED=OFF`.

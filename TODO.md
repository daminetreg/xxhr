# Tasks

## bearssl stream

- [ ] Make the stream_bearssl.hpp really compile
- [ ] Implement an asio::ssl::context which is based on bearssl
  - [ ] Strikethrough bad implementation to get a first test
  - [ ] Complete impl with certificate loading ...

### Improvements
- Adds an std::variant<br_ssl_client_context, br_ssl_server_context> to determine which type should be used for server / client work. (Replace the br_ssl_client_context).

## Side task
- Fix mini bug in asio : it always returns sys_error in engine.ipp and PR it.

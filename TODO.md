# Tasks

## bearssl stream
- Adds an std::variant<br_ssl_client_context, br_ssl_server_context> to determine which type should be used for server / client work. (Replace the br_ssl_client_context).
- Fix mini bug in asio : it always returns sys_error in engine.ipp and PR it.

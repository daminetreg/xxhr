if (DEFINED EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-g4) # Generates emscripten C++ sourcemaps
  endif()
endif()

# \brief add_example(NAME <test name>)
# A <test name>.cpp has to exists for the test to be valid. 
# The main method <test name>.cpp should return non-zero on failure.
function(add_example)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs LIBS)
  cmake_parse_arguments(add_example "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(${add_example_NAME} ${add_example_NAME}.cpp)
  target_link_libraries(${add_example_NAME} cpp-js::xxhr)
  add_test(NAME ${add_example_NAME} COMMAND $<TARGET_FILE:${add_example_NAME}>)

  if (DEFINED EMSCRIPTEN)
    set_target_properties(${add_example_NAME}
      PROPERTIES SUFFIX ".html"
      LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1")
  endif (DEFINED EMSCRIPTEN)

endfunction(add_example)

add_example(NAME sync)
add_example(NAME introductory)
add_example(NAME getting_started)
add_example(NAME retrying)
add_example(NAME parameters)
add_example(NAME authentication)
add_example(NAME multipart)

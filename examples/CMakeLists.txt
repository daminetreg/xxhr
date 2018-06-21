if (DEFINED EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-g4) # Generates emscripten C++ sourcemaps
  endif()
endif()

add_executable(xxhr_main main.cpp)
target_link_libraries(xxhr_main cpp-js::xxhr)

add_executable(xxhr_sync main_syncapi.cpp)
target_link_libraries(xxhr_sync cpp-js::xxhr)

add_executable(xxhr_introductory introductory.cpp)
target_link_libraries(xxhr_introductory cpp-js::xxhr)

if (DEFINED EMSCRIPTEN)
set_target_properties(xxhr_main 
  PROPERTIES SUFFIX ".html"
  LINK_FLAGS " -g4 --bind -s  NO_EXIT_RUNTIME=1")

set_target_properties(xxhr_sync 
  PROPERTIES SUFFIX ".html"
  LINK_FLAGS " --bind")

set_target_properties(xxhr_introductory 
  PROPERTIES SUFFIX ".html"
  LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1")

endif (DEFINED EMSCRIPTEN)

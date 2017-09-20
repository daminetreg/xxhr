if (DEFINED EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-g4) # Generates emscripten C++ sourcemaps
  endif()
endif()


add_executable(xxhr_main main.cpp)
target_link_libraries(xxhr_main cpp-js::xxhr)

add_executable(xxhr_sync main_syncapi.cpp)
target_link_libraries(xxhr_main cpp-js::xxhr)

add_executable(xxhr_introductory introductory.cpp)
target_link_libraries(xxhr_main cpp-js::xxhr)

find_package(Threads REQUIRED)

add_executable(xxhr_ssl main_ssl.cpp)
target_compile_definitions(xxhr_ssl PRIVATE -DBOOST_ERROR_CODE_HEADER_ONLY -DBOOST_SYSTEM_NO_DEPRECATED)
target_include_directories(xxhr_ssl PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(xxhr_ssl cpp-js::xxhr ${CMAKE_THREAD_LIBS_INIT})


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

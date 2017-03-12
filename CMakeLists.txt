cmake_minimum_required(VERSION 3.0.0)
project (xxhr)

add_definitions(-std=c++14)
set (CMAKE_CXX_STANDARD 14)

# Boost
include_directories (AFTER /opt/libs/boost_1_61_0/)
link_directories(/opt/libs/boost_1_61_0/stage-emscripten/lib/)

# Beast
include_directories(/opt/libs/Beast.js/include/)

# js::bind
include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/../js-bind)
include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/../js-bind/all-inclusive)

if (DEFINED EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-g)
    add_definitions(-g4) # Generates emscripten C++ sourcemaps
  endif()
endif()


include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})

add_executable(xxhr main.cpp)
add_executable(xxhr_sync main_syncapi.cpp)
target_link_libraries(xxhr boost_system)

if (DEFINED EMSCRIPTEN)
set_target_properties(xxhr 
  PROPERTIES SUFFIX ".html"
  LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1")

set_target_properties(xxhr_sync 
  PROPERTIES SUFFIX ".html"
  LINK_FLAGS " --bind")

endif (DEFINED EMSCRIPTEN)

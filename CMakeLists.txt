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
file (GLOB_RECURSE SRC_FILES main.cpp)

if (DEFINED EMSCRIPTEN)
  

  #add_library(xxhr SHARED ${SRC_FILES})
  add_executable(xxhr ${SRC_FILES})

else (DEFINED EMSCRIPTEN)
  add_executable(xxhr ${SRC_FILES})
endif(DEFINED EMSCRIPTEN)

target_link_libraries(xxhr boost_system)

#file (COPY ${CMAKE_CURRENT_LIST_DIR}/Arial DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

if (DEFINED EMSCRIPTEN)
  set_target_properties(xxhr 
    PROPERTIES SUFFIX ".html"
    LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1 -g4")
  em_link_js_library(xxhr ${libraryJsFiles})
  # add -O2 to minify
endif (DEFINED EMSCRIPTEN)


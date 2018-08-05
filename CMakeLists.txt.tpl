
# This is an #inglued <> template !
#
# The template is processed by http://mustache.github.io/, more info about
# syntax here : http://mustache.github.io/mustache.5.html 
#
# You can access the following variables : 
# * {{org}} : github organization name
# * {{project}} : current project name
# * {{project_srcs}} : current project srcs folder.
#
# * {{#deps}} {{/deps}} : all deps direct and transitive 
#   - {{cmake_package_name}} : The cmake package name from the cmake_package_map otherwise: {{org}}
#   - {{cmake_target_name}} : The cmake target name from cmake_package_map otherwise: {{cmake_package_name}}::{{name}}
#   - {{org}} : the github organization name
#   - {{name}} : the dependency repository name
#   - {{ref}} : tag or branch wished for the dep
#   - {{include_path}} : the path you specified in deps/inglued -I
#   - {{include_path_end_backslash}} : same as above but with a guaranteed end slash.
#

##### PLATFORM Provided Libraries #####
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.10.tar.gz"
    SHA1 "1233baf66650caf986d509b6985bcfa23cd7c519"
)
##### PLATFORM Provided Libraries #####

cmake_minimum_required(VERSION 3.5.0)

project({{project}} VERSION "0.0.2")
enable_testing()

# Compile with shipped-with headers or without 
option(INGLUED "Enable use of #inglued shipped with dependencies." ON)

# Compile unit tests
option(UNIT_TESTS "Enable Unit Testing" OFF)

add_compile_options(-std=c++17)

# Warning as errors to ensure {{project}} quality
string(TOUPPER "${CMAKE_CXX_COMPILER_ID}" COMPILER_IN_USE)
if ("${COMPILER_IN_USE}" STREQUAL "GNU" OR "${COMPILER_IN_USE}" MATCHES "CLANG")
	add_definitions(
    -Wall
#		-Werror
		-Wno-unused-local-typedefs
		-Wno-unused-variable
  )
endif()

# XXX: nxxm cannot ship this for the moment. An option would be to ship it with nxxm until headerizer is finished.
add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)
find_package(Threads REQUIRED)

hunter_add_package(OpenSSL)
find_package(OpenSSL REQUIRED)


{{#deps}}
# {{name}}
if (INGLUED)
  include_directories(AFTER deps/{{org}}/{{name}}/{{include_path}})
else ()
  find_package({{cmake_package_name}} REQUIRED)
endif()

{{/deps}}

# Define header only library
include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})
add_library({{project}} INTERFACE)
add_library({{org}}::{{project}} ALIAS {{project}})

target_link_libraries({{project}} INTERFACE OpenSSL::SSL OpenSSL::Crypto ${CMAKE_THREAD_LIBS_INIT})

set(include_install_dir "include")

if (INGLUED)

  target_include_directories({{project}} INTERFACE 
    # Transitive libraries location once installed
    $<INSTALL_INTERFACE:${include_install_dir}/{{project_srcs}}/deps> 
  )


else()

  target_link_libraries({{project}} INTERFACE 
    {{#deps}}
      {{cmake_target_name}}
    {{/deps}}
  )

endif()


if( UNIT_TESTS )
# add_subdirectory(test)
 add_subdirectory(examples)
endif()

# IDE Supports (XCode, VS, QtCreator they don't list headers)
file (GLOB_RECURSE HPP_FILES {{project_srcs}}/*.h*)
add_custom_target(sources SOURCES ${HPP_FILES})

# Installing

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/modules/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)


# Targets:
install(
    TARGETS {{project}}
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
    DIRECTORY {{project_srcs}}
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.[ih]*"
)

# Install the deps when run in INGLUED mode
if (INGLUED)
  {{=<% %>=}}

  <%#deps%>

  install(
      DIRECTORY deps/<%org%>/<%name%>/<%include_path_end_backslash%>
      DESTINATION ${include_install_dir}/<%project_srcs%>/deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/<%org%>/<%name%>/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )

  <%/deps%>

  <%={{ }}=%>
endif(INGLUED)

# Config
#   * <prefix>/lib/cmake/{{project}}/{{project}}Config.cmake
#   * <prefix>/lib/cmake/{{project}}/{{project}}ConfigVersion.cmake
#   * <prefix>/lib/cmake/{{project}}/{{project}}Targets.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)


cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

project(Config LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
if(MSVC_IDE)
  set(CMAKE_CXX_FLAGS_RELEASE "-Ox")
endif()

include(cmake-tools/GroupSources.cmake)

set(OCRA_CONFIG_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cpp)
set(OCRA_CONFIG_HEADER_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/include/Config.hpp)

add_library(Config STATIC ${OCRA_CONFIG_SOURCE_FILES} ${OCRA_CONFIG_HEADER_FILES})
target_include_directories(Config PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC_IDE)
  # Run macro
  GroupSources(src)
  GroupSources(include)
endif()

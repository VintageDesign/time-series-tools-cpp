# Forces variables with set() to override option() variables later defined with
# the same name in child projects added with add_subdiretory()
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

cmake_minimum_required(VERSION 3.16)

project(aic LANGUAGES CXX)

set(IS_MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(IS_MAIN_PROJECT ON)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(IS_MAIN_PROJECT)
  set(CMAKE_CXX_STANDARD 11)
endif()

option(BUILD_TESTS "Builds the test executable for the project"
       ${IS_MAIN_PROJECT})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)

add_subdirectory(tests)

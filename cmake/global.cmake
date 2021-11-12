# include_guard: https://cmake.org/cmake/help/latest/command/include_guard.html
include_guard()

# env

if(MSVC OR MSYS OR MINGW)
  set(HOST_OS win)
  set(HOST_OS_WIN true)
elseif(APPLE)
  set(HOST_OS mac)
  set(HOST_OS_MAC true)
elseif(UNIX)
  set(HOST_OS linux)
  set(HOST_OS_LINUX true)
else()
  message(FATAL_ERROR "Unsupported OS")
endif()

include(${CMAKE_CURRENT_LIST_DIR}/global/target_arch.cmake)
target_architecture(TARGET_ARCH)

set(_arms arm armv5 armv6 armv7)
set(_arm64s armv8 arm64)
if(TARGET_ARCH STREQUAL "i386")
  set(HOST_ARCH x86)
elseif(TARGET_ARCH STREQUAL "x86_64")
  set(HOST_ARCH x64)
elseif(TARGET_ARCH IN_LIST _arms)
  set(HOST_ARCH arm)
elseif(TARGET_ARCH IN_LIST _arm64s)
  set(HOST_ARCH arm64)
else()
  message(FATAL_ERROR "Unsupported arch")
endif()
unset(_arms)
unset(_arm64s)

message(STATUS "HOST_OS: ${HOST_OS}")
message(STATUS "HOST_ARCH: ${HOST_ARCH}")

# macros

include(${CMAKE_CURRENT_LIST_DIR}/global/macros.cmake)

# modules

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

# cmake

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

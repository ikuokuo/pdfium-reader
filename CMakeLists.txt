cmake_minimum_required(VERSION 3.10)
project(pdfium-reader VERSION 1.0.0 LANGUAGES C CXX)

set(MY_NAME ${PROJECT_NAME})
set(MY_ROOT ${PROJECT_SOURCE_DIR})

include(${MY_ROOT}/cmake/global.cmake)

# packages

## threads

find_package(Threads REQUIRED)

## pdfium

set(PDFium_DIR ${MY_ROOT}/third_party/pdfium-${HOST_OS}-${HOST_ARCH})
message(STATUS "PDFium libs path: ${PDFium_DIR}")
if(IS_DIRECTORY ${PDFium_DIR})
  message(STATUS "PDFium libs found")
  find_package(PDFium REQUIRED)
  #message(STATUS "PDFium_INCLUDE_DIR: ${PDFium_INCLUDE_DIR}")
  #message(STATUS "PDFium_LIBRARY: ${PDFium_LIBRARY}")
else()
  message(FATAL_ERROR "PDFium libs not found, download them from here: https://github.com/bblanchon/pdfium-binaries")
endif()

# targets

include_directories(
)

set(MY_OUTPUT "${MY_ROOT}/_output")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MY_OUTPUT}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MY_OUTPUT}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_OUTPUT}/bin")
foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
  string(TOUPPER ${CONFIG} CONFIG)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endforeach()

set(MY_TARGETS )

## pdfium_reader

# samples

add_subdirectory(${MY_ROOT}/samples)

# install

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
#message(${CMAKE_INSTALL_PREFIX})
#message(${CMAKE_INSTALL_INCLUDEDIR})
#message(${CMAKE_INSTALL_BINDIR})
#message(${CMAKE_INSTALL_LIBDIR})
#message(${CMAKE_INSTALL_CMAKEDIR})

set(CMAKE_MACOSX_RPATH 1)
#list(APPEND CMAKE_BUILD_RPATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
#list(APPEND CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")

install(TARGETS ${MY_TARGETS}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

## pdfium

#install(IMPORTED_RUNTIME_ARTIFACTS pdfium)
install(FILES ${PDFium_LIBRARY} DESTINATION ${CMAKE_INSTALL_BINDIR})

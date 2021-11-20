cmake_minimum_required(VERSION 3.10)
project(pdfium-reader VERSION 1.0.0 LANGUAGES C CXX)

set(MY_NAME ${PROJECT_NAME})
set(MY_ROOT ${PROJECT_SOURCE_DIR})
get_filename_component(MY_3RDPARTY "${MY_ROOT}/third_party" REALPATH)

include(${MY_ROOT}/cmake/global.cmake)

set(MY_APPNAME "PDFium Reader")
set(MY_VERSION ${PROJECT_VERSION})
set_version_values(MY_VERSION)

# configs

configure_file(src/config.h.in ${MY_ROOT}/src/config.h @ONLY)

# packages

## threads

find_package(Threads REQUIRED)

## opengl

include(${MY_ROOT}/cmake/detect_gl.cmake)

## imgui

set(ImGui_DIR "${MY_3RDPARTY}/imgui")
message(STATUS "ImGui_DIR: ${ImGui_DIR}")

set(ImGui_SRCS
  ${ImGui_DIR}/imgui.cpp
  ${ImGui_DIR}/imgui_demo.cpp
  ${ImGui_DIR}/imgui_draw.cpp
  ${ImGui_DIR}/imgui_tables.cpp
  ${ImGui_DIR}/imgui_widgets.cpp
  ${ImGui_DIR}/backends/imgui_impl_glfw.cpp
  ${ImGui_DIR}/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${ImGui_SRCS})
target_include_directories(imgui PUBLIC
  "$<BUILD_INTERFACE:${ImGui_DIR}>"
  "$<BUILD_INTERFACE:${ImGui_DIR}/backends>"
)

## Native File Dialog

set(NFD_DIR "${MY_3RDPARTY}/nativefiledialog")
message(STATUS "NFD_DIR: ${NFD_DIR}")
add_subdirectory(${NFD_DIR})

## pdfium

set(PDFium_DIR ${MY_3RDPARTY}/pdfium-${HOST_OS}-${HOST_ARCH})
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
  ${MY_ROOT}/src
  ${GL_INCLUDE_DIRS}
)

set(MY_OUTPUT "${MY_ROOT}/_output")
set_outdir(
  ARCHIVE "${MY_OUTPUT}/lib"
  LIBRARY "${MY_OUTPUT}/lib"
  RUNTIME "${MY_OUTPUT}/bin"
)

set(MY_TARGETS )

include(${MY_ROOT}/src/CMakeLists.txt)

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

# samples

add_subdirectory(${MY_ROOT}/samples)

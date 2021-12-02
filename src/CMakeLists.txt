set(MY_SRC ${CMAKE_CURRENT_LIST_DIR})

# pdfium_reader

list(APPEND MY_TARGETS pdfium_reader)

set(_srcs ${MY_SRC}/main.cc)
include(${MY_SRC}/base/CMakeLists.txt)
include(${MY_SRC}/pdf/CMakeLists.txt)
include(${MY_SRC}/ui/CMakeLists.txt)

add_executable(pdfium_reader ${_srcs})
target_link_libraries(pdfium_reader ${GL_LIBS} imgui nfd pdfium)
target_copy_libraries(pdfium_reader pdfium)
target_include_directories(pdfium_reader PRIVATE
  ${MY_ROOT}/third_party/stb
)
target_compile_options(pdfium_reader PRIVATE
  "-Wno-missing-field-initializers"
)
unset(_srcs)

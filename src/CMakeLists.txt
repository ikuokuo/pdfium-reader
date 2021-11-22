set(MY_SRC ${CMAKE_CURRENT_LIST_DIR})

# pdfium_reader

list(APPEND MY_TARGETS pdfium_reader)

set(_srcs ${MY_SRC}/main.cc)
include(${MY_SRC}/ui/CMakeLists.txt)
add_executable(pdfium_reader ${_srcs})
target_link_libraries(pdfium_reader ${GL_LIBS} imgui nfd)
unset(_srcs)

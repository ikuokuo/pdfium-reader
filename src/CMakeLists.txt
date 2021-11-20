set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

# pdfium_reader

list(APPEND MY_TARGETS pdfium_reader)

add_executable(pdfium_reader
  ${MY_CURR}/base/glfw_base.cc
  ${MY_CURR}/base/imgui_base.cc
  ${MY_CURR}/ui/native/filedialog_nfd.cc
  ${MY_CURR}/ui/native/native_nfd.cc
  ${MY_CURR}/ui/ui.cc
  ${MY_CURR}/main.cc
)
target_link_libraries(pdfium_reader ${GL_LIBS} imgui nfd)

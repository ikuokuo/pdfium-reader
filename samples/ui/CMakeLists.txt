set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

get_filename_component(MY_NAME ${MY_CURR} NAME)
set_outdir(
  ARCHIVE "${MY_OUTPUT}/lib/${MY_NAME}"
  LIBRARY "${MY_OUTPUT}/lib/${MY_NAME}"
  RUNTIME "${MY_OUTPUT}/bin/${MY_NAME}"
)

set(_demos)

# glfw_demo

find_package(GLEW)

if(GLEW_FOUND)
  list(APPEND _demos glfw_demo)

  add_executable(glfw_demo
    ${MY_ROOT}/src/base/glfw_base.cc
    ${MY_CURR}/glfw_demo.cc
  )
  target_link_libraries(glfw_demo ${GL_LIBS} GLEW::GLEW)
else()
  status("Skip glfw_demo, as GLEW not found")
endif()

# imgui_demo

list(APPEND _demos imgui_demo)

add_executable(imgui_demo
  ${MY_ROOT}/src/base/glfw_base.cc
  ${MY_ROOT}/src/base/imgui_base.cc
  ${MY_CURR}/imgui_demo.cc
)
target_link_libraries(imgui_demo ${GL_LIBS} imgui)

# install

if(_demos)
  install(TARGETS ${_demos}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${MY_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MY_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MY_NAME}
  )
endif()

unset(_demos)

set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

get_filename_component(MY_NAME ${MY_CURR} NAME)
set_outdir(
  ARCHIVE "${MY_OUTPUT}/lib/${MY_NAME}"
  LIBRARY "${MY_OUTPUT}/lib/${MY_NAME}"
  RUNTIME "${MY_OUTPUT}/bin/${MY_NAME}"
)

# targets

set(_names
  pdf_info
  pdf_render
)

foreach(name IN LISTS _names)
  add_executable(${name}
    ${MY_CURR}/${name}.cc
    ${MY_CURR}/helper.cc
  )
  target_link_libraries(${name} pdfium)
  target_copy_libraries(${name} pdfium)
  target_include_directories(${name} PRIVATE
    ${MY_ROOT}/third_party/stb
  )
  target_compile_options(${name} PRIVATE
    "-Wno-missing-field-initializers"
  )
endforeach()

# install

install(TARGETS ${_names}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${MY_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MY_NAME}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${MY_NAME}
)

unset(_names)

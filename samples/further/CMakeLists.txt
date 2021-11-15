set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

# targets

set(names
  pdf_info
  pdf_render
)

foreach(name IN LISTS names)
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

install(TARGETS ${targets}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

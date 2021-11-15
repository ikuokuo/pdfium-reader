set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

# targets

set(names
  pdf_info
)

foreach(name IN LISTS names)
  add_executable(${name}
    ${MY_CURR}/${name}.cc
    ${MY_CURR}/helper.cc
  )
  target_link_libraries(${name} pdfium)
  target_copy_libraries(${name} pdfium)
endforeach()

# install

install(TARGETS ${targets}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

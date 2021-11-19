set(MY_CURR ${CMAKE_CURRENT_LIST_DIR})

# pdfium_start

add_executable(pdfium_start
  ${MY_CURR}/pdfium_start.c
)
target_link_libraries(pdfium_start pdfium)
target_copy_libraries(pdfium_start pdfium)

# samples/pdf

add_subdirectory(${MY_CURR}/pdf)

# install

install(TARGETS pdfium_start
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

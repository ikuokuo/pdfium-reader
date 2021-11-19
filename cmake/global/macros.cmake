include(CMakeParseArguments)

# target_copy_libraries(target items...)
macro(target_copy_libraries target)
  foreach(arg IN ITEMS ${ARGN})
    add_custom_command(
      TARGET ${target} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${arg}> $<TARGET_FILE_DIR:${target}>
    )
  endforeach()
endmacro()

# set_outdir(ARCHIVE outdir LIBRARY outdir RUNTIME outdir [KEEP_CONFIG])
macro(set_outdir)
  set(options KEEP_CONFIG)
  set(oneValueArgs ARCHIVE LIBRARY RUNTIME)
  set(multiValueArgs)
  cmake_parse_arguments(THIS "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  if(THIS_ARCHIVE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${THIS_ARCHIVE})
  endif()
  if(THIS_LIBRARY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${THIS_LIBRARY})
  endif()
  if(THIS_RUNTIME)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${THIS_RUNTIME})
  endif()

  if(NOT THIS_KEEP_CONFIG)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
      string(TOUPPER ${CONFIG} CONFIG)
      set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
      set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    endforeach()
  endif()
endmacro()

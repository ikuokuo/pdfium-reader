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

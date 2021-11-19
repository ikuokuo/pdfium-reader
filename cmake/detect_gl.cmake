include(${CMAKE_CURRENT_LIST_DIR}/global.cmake)

if(NOT MY_3RDPARTY)
  set(MY_3RDPARTY ${CMAKE_CURRENT_LIST_DIR}/../3rdparty)
  message(WARNING "MY_3RDPARTY not found, try set: ${MY_3RDPARTY}")
endif()
if(NOT IS_DIRECTORY "${MY_3RDPARTY}")
  message(FATAL_ERROR "MY_3RDPARTY not exists: ${MY_3RDPARTY}")
endif()

# glfw3

find_package(GLFW3 REQUIRED)

# glm

if(NOT IS_DIRECTORY "${MY_3RDPARTY}/glm")
  message(FATAL_ERROR "MY_3RDPARTY/glm not exists: ${MY_3RDPARTY}/glm")
endif()

# GL_INCLUDE_DIRS

set(GL_INCLUDE_DIRS
  ${GLFW3_INCLUDE_DIRS}
  ${MY_3RDPARTY}/glm
)

# GL_LIBS

if(HOST_OS STREQUAL "win")
  set(GL_LIBS glfw3 opengl32)
elseif(HOST_OS STREQUAL "linux")
  set(OpenGL_GL_PREFERENCE GLVND)
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(GL_LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread)
  set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(HOST_OS STREQUAL "mac")
  message(STATUS "CMAKE_OSX_SYSROOT: ${CMAKE_OSX_SYSROOT}")
  include_directories(/System/Library/Frameworks)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(OpenGL_LIBRARY OpenGL)
  find_library(IOKit_LIBRARY IOKit)
  find_library(CoreVideo_LIBRARY CoreVideo)
  mark_as_advanced(COCOA_LIBRARY OpenGL_LIBRARY)
  set(GL_APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
  set(GL_APPLE_LIBS ${GL_APPLE_LIBS} ${GLFW3_LIBRARY})
  set(GL_LIBS ${GL_LIBS} ${GL_APPLE_LIBS})
else()
  set(GL_LIBS )
endif()

list(APPEND GL_LIBS)

# message

message(STATUS "GL_INCLUDE_DIRS: ${GL_INCLUDE_DIRS}")
message(STATUS "GL_LIBS: ${GL_LIBS}")

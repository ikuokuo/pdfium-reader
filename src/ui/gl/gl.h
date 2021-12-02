#pragma once

#define MY_GL_INCLUDE_GLCOREARB
#define MY_GL_INCLUDE_GLEXT
// #define MY_GL_INCLUDE_GLU
// #define MY_GL_INCLUDE_NONE

#if defined(__APPLE__)
# define GL_SILENCE_DEPRECATION
# if defined(MY_GL_INCLUDE_GLCOREARB)
#  include <OpenGL/gl3.h>
#  if defined(MY_GL_INCLUDE_GLEXT)
#   include <OpenGL/gl3ext.h>
#  endif
# elif !defined(MY_GL_INCLUDE_NONE)
#  if !defined(MY_GL_INCLUDE_GLEXT)
#   define GL_GLEXT_LEGACY
#  endif
#  include <OpenGL/gl.h>
# endif
# if defined(MY_GL_INCLUDE_GLU)
#  include <OpenGL/glu.h>
# endif
#else
# if defined(MY_GL_INCLUDE_GLCOREARB)
#  include <GL/glcorearb.h>
# elif !defined(MY_GL_INCLUDE_NONE)
#  include <GL/gl.h>
#  if defined(MY_GL_INCLUDE_GLEXT)
#   include <GL/glext.h>
#  endif
# endif
# if defined(MY_GL_INCLUDE_GLU)
#  include <GL/glu.h>
# endif
#endif

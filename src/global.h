#pragma once

// os

#ifdef _WIN32
# define MY_OS_WIN
# ifdef _WIN64
#  define MY_OS_WIN64
# else
#  define MY_OS_WIN32
# endif
# if defined(__MINGW32__) || defined(__MINGW64__)
#  define MY_OS_MINGW
#  ifdef __MINGW64__
#   define MY_OS_MINGW64
#  else
#   define MY_OS_MINGW32
#  endif
# elif defined(__CYGWIN__) || defined(__CYGWIN32__)
#  define MY_OS_CYGWIN
# endif
#elif __APPLE__
# include <TargetConditionals.h>
# if TARGET_IPHONE_SIMULATOR
#  define MY_OS_IPHONE
#  define MY_OS_IPHONE_SIMULATOR
# elif TARGET_OS_IPHONE
#  define MY_OS_IPHONE
# elif TARGET_OS_MAC
#  define MY_OS_MAC
# else
#  error "Unknown Apple platform"
# endif
#elif __ANDROID__
# define MY_OS_ANDROID
#elif __linux__
# define MY_OS_LINUX
#elif __unix__
# define MY_OS_UNIX
#elif defined(_POSIX_VERSION)
# define MY_OS_POSIX
#else
# error "Unknown compiler"
#endif

#ifndef MY_OS_SEP
# if defined(MY_OS_WIN) && !defined(MY_OS_MINGW)
#  define MY_OS_SEP '\\'
#  define MY_OS_SEP_STR "\\"
# else
#  define MY_OS_SEP '/'
#  define MY_OS_SEP_STR "/"
# endif
#endif

// export

#if defined(MY_OS_WIN)
# define MY_DECL_EXPORT __declspec(dllexport)
# define MY_DECL_IMPORT __declspec(dllimport)
# define MY_DECL_HIDDEN
#else
# define MY_DECL_EXPORT __attribute__((visibility("default")))
# define MY_DECL_IMPORT __attribute__((visibility("default")))
# define MY_DECL_HIDDEN __attribute__((visibility("hidden")))
#endif

#ifdef MY_EXPORTS
# define MY_API MY_DECL_EXPORT
#else
# define MY_API MY_DECL_IMPORT
#endif

#ifdef __cplusplus
# define MY_EXTERN_C       extern "C"
# define MY_EXTERN_C_BEGIN extern "C" {
# define MY_EXTERN_C_END   }
#else
# define MY_EXTERN_C       /* Nothing */
# define MY_EXTERN_C_BEGIN /* Nothing */
# define MY_EXTERN_C_END   /* Nothing */
#endif

// ns

#ifndef MY_NAMESPACE_BEGIN
# define MY_NAMESPACE_BEGIN(name) namespace name {
#endif
#ifndef MY_NAMESPACE_END
# define MY_NAMESPACE_END(name) }
#endif

#ifndef MY_NAMESPACE
# define MY_NAMESPACE ikuokuo
#endif
#ifndef MY_BEGIN_NAMESPACE
# define MY_BEGIN_NAMESPACE MY_NAMESPACE_BEGIN(MY_NAMESPACE)
#endif
#ifndef MY_END_NAMESPACE
# define MY_END_NAMESPACE MY_NAMESPACE_END(MY_NAMESPACE)
#endif
#ifndef MY_USE_NAMESPACE
# define MY_USE_NAMESPACE using namespace ::MY_NAMESPACE;  // NOLINT
#endif

// helper

#ifndef MY_STRINGIFY_HELPER
# define MY_STRINGIFY_HELPER(X) #X
#endif
#ifndef MY_STRINGIFY
# define MY_STRINGIFY(X) MY_STRINGIFY_HELPER(X)
#endif

#ifndef MY_DISABLE_COPY
# define MY_DISABLE_COPY(Class) \
  Class(const Class &) = delete; \
  Class &operator=(const Class &) = delete;
#endif

#ifndef MY_DISABLE_MOVE
# define MY_DISABLE_MOVE(Class) \
  Class(Class &&) = delete; \
  Class &operator=(Class &&) = delete;
#endif

#ifndef MY_DISABLE_COPY_MOVE
# define MY_DISABLE_COPY_MOVE(Class) \
  MY_DISABLE_COPY(Class) \
  MY_DISABLE_MOVE(Class)
#endif

#ifndef MY_UNUSED
# define MY_UNUSED(x) (void)x;
#endif

#ifndef MY_UNKNOWN
# define MY_UNKNOWN "UNKNOWN"
#endif

#ifndef MY_TRUE_STR
# define MY_TRUE_STR "TRUE"
#endif
#ifndef MY_FALSE_STR
# define MY_FALSE_STR "FALSE"
#endif
#ifndef MY_BOOL_STR
# define MY_BOOL_STR(b) (b ? MY_TRUE_STR : MY_FALSE_STR)
#endif

#ifndef MY_ON_STR
# define MY_ON_STR "ON"
#endif
#ifndef MY_OFF_STR
# define MY_OFF_STR "OFF"
#endif
#ifndef MY_SWITCH_STR
# define MY_SWITCH_STR(b) (b ? MY_ON_STR : MY_OFF_STR)
#endif

#include "config.h"

#ifndef STATIC_ALLOCATOR_COMPILER_MACROS_H
#define STATIC_ALLOCATOR_COMPILER_MACROS_H

#if defined(__clang__)
#  define COMPILER_CLANG
#elif defined(__INTEL_COMPILER)
#  define COMPILER_ICC
#elif defined(__GNUC__)
#  define COMPILER_GCC
#elif defined(_MSC_VER) || defined(_MSVC_LANG)
#  define COMPILER_MSVC
#else
#  define COMPILER_UNKNOWN
#endif

#if defined(COMPILER_MSVC)
#  if !defined(_MSVC_LANG)
#    define COMPILER_CPP_VERSION __cplusplus
#  else
#    define COMPILER_CPP_VERSION _MSVC_LANG
#  endif
#else
#  define COMPILER_CPP_VERSION __cplusplus
#endif

#ifdef UTIL_XCAT
#  undef UTIL_XCAT
#endif
#ifdef UTIL_CAT
#  undef UTIL_CAT
#endif

#define UTIL_XCAT(x, y) x ## y
#define UTIL_CAT(x, y) UTIL_XCAT(x, y)

#endif //STATIC_ALLOCATOR_COMPILER_MACROS_H

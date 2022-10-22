#ifndef STATIC_ALLOCATOR_PASSERT_HPP
#define STATIC_ALLOCATOR_PASSERT_HPP

#include "../detail/compiler_macros.h"

#ifdef COMPILER_MSVC
#  include <assert.h>
#else
#  include <cassert>
#endif

#ifdef __APPLE__
#  define assert_failure(output) \
    __assert_rtn(__func__, __FILE__, __LINE__, output)
#else
#  define assert_failure(output) \
    __assert_fail(output, __FILE__, __LINE__, __extension__ __PRETTY_FUNCTION__)
#endif

#if defined(COMPILER_GCC) || defined(COMPILER_CLANG) || defined(COMPILER_ICC)

#define pretty_assert(cond, output)             \
    do {                                        \
        (static_cast<bool>(cond) ?               \
        void(0) : assert_failure(output));     \
    } while(0)

#elif defined(COMPILER_MSVC)

#  define IMPL_WIDEN(str) UTIL_CAT(L, str)
#define pretty_assert(cond, output)             \
    do {                                        \
        (static_cast<bool>(cond) ?               \
        void(0) : _wassert (                    \
            $WIDEN(output), $WIDEN(__FILE__),    \
            __LINE__));                          \
    } while(0)

#else
#  define pretty_assert(cond, output) assert(static_cast<bool>(cond) && output)
#endif

#endif //STATIC_ALLOCATOR_PASSERT_HPP

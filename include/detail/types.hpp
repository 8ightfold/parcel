#ifndef STATIC_ALLOCATOR_TYPES_HPP
#define STATIC_ALLOCATOR_TYPES_HPP

#include <type_traits>
#include <utility>
#include "compiler_macros.h"

namespace parcel {
#if defined(COMPILER_GCC) || defined(COMPILER_CLANG) || defined(COMPILER_ICC)
#  if defined(__SIZEOF_INT128__)
    using imax_t = __int128;
    using umax_t = unsigned __int128;
#  else
    using imax_t = std::make_signed<std::size_t>;
    using umax_t = std::size_t;
#  endif
#else
    using imax_t = std::make_signed<std::size_t>;
    using umax_t = std::size_t;
#endif

    using byte_t = unsigned char;

    template <parcel::umax_t N>
    using integral_t = std::integral_constant<parcel::umax_t, N>;

    template <parcel::umax_t...NN>
    using seq_t = std::integer_sequence<parcel::umax_t, NN...>;

    template <parcel::umax_t N>
    using make_seq_t = decltype(std::make_integer_sequence<parcel::umax_t, N>());

    template <typename T>
    concept novoid = not std::is_void_v<T>;

    template <typename T>
    struct type_wrapper {
        using type = T;
    };
}

#endif //STATIC_ALLOCATOR_TYPES_HPP

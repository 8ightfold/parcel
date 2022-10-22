#ifndef STATIC_ALLOCATOR_PARCEL_STL_HPP
#define STATIC_ALLOCATOR_PARCEL_STL_HPP

#include "heap.hpp"
#include "static_allocator.hpp"

#include <string>
#include <vector>

namespace parcel {
    template <typename CharType>
    using basic_string = typename std::basic_string<CharType, std::char_traits<CharType>, parcel::allocator<CharType>>;

    using string	= basic_string<char>;
    using wstring	= basic_string<wchar_t>;
    using u8string	= basic_string<char8_t>;
    using u16string	= basic_string<char16_t>;
    using u32string = basic_string<char32_t>;

    template <typename T>
    using vector = typename std::vector<T, parcel::allocator<T>>;
}

#endif //STATIC_ALLOCATOR_PARCEL_STL_HPP

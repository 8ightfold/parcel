#ifndef STATIC_ALLOCATOR_PARCEL_STL_HPP
#define STATIC_ALLOCATOR_PARCEL_STL_HPP

#include "heap.hpp"
#include "static_allocator.hpp"

#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace parcel {
    template <typename T>
    using deque = std::deque<T, parcel::allocator<T>>;

    template <typename T>
    using forward_list = std::forward_list<T, parcel::allocator<T>>;

    template <typename T>
    using list = std::list<T, parcel::allocator<T>>;

    template <typename K, typename T, typename Compare = std::less<K>>
    using map = std::map<K, T, Compare, parcel::allocator<std::pair<const K, T>>>;

    template <typename T>
    using queue = std::queue<T, parcel::deque<T>>;

    template <typename K, typename Compare = std::less<K>>
    using set = std::set<K, Compare, parcel::allocator<K>>;

    template <typename T>
    using stack = std::stack<T, parcel::deque<T>>;

    template <typename CharType>
    using basic_string = typename std::basic_string<CharType, std::char_traits<CharType>, parcel::allocator<CharType>>;

    using string	= basic_string<char>;
    using wstring	= basic_string<wchar_t>;
    using u8string	= basic_string<char8_t>;
    using u16string	= basic_string<char16_t>;
    using u32string = basic_string<char32_t>;

    template <typename K, typename T, typename Hash = std::hash<K>, typename Pred = std::equal_to<K>>
    using unordered_map = std::unordered_map<K, T, Hash, Pred, parcel::allocator<std::pair<const K, T>>>;

    template <typename K, typename Hash = std::hash<K>, typename KeyEqual = std::equal_to<K>>
    using unordered_set = std::unordered_set<K, Hash, KeyEqual, parcel::allocator<K>>;

    template <typename T>
    using vector = typename std::vector<T, parcel::allocator<T>>;
}

#endif //STATIC_ALLOCATOR_PARCEL_STL_HPP

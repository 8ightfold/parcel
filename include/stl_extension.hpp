#ifndef STATIC_ALLOCATOR_STL_EXTENSION_HPP
#define STATIC_ALLOCATOR_STL_EXTENSION_HPP

#include "block_head.hpp"
#include "static_allocator.hpp"

#include <vector>

namespace parcel {
    template <typename T>
    using vector = typename std::vector<T, parcel::allocator<T>>;
}

#endif //STATIC_ALLOCATOR_STL_EXTENSION_HPP

#ifndef STATIC_ALLOCATOR_STATIC_ALLOCATOR_HPP
#define STATIC_ALLOCATOR_STATIC_ALLOCATOR_HPP

#include <memory>
#include "block_head.hpp"

namespace parcel {
    template <typename T>
    struct allocator {
        using value_type = T;
        using size_type = parcel::umax_t;
        using difference_type = std::ptrdiff_t;

        using value_type_extents = T[];

        allocator() = default;

        template <class U>
        constexpr
        allocator(const allocator<U>&) noexcept {}

        [[nodiscard]]
        value_type* allocate(size_type n) noexcept {
            pretty_assert(n < (static_cast<std::size_t>(-1) / sizeof(value_type)),
                "parcel::allocator<T>::allocate(umax_t n):"
                " 'n' exceeds maximum supported size.");

            void* data = allocate_block(type_wrapper<value_type_extents>{}, n);
            return reinterpret_cast<value_type*>(data);
        }

        void deallocate(value_type* addr, size_type n) {
            deallocate_block(type_wrapper<value_type_extents>{}, addr, n);
        }
    };

    template<class T, class U>
    bool operator==(const allocator <T>&, const allocator <U>&) { return true; }

    template<class T, class U>
    bool operator!=(const allocator <T>&, const allocator <U>&) { return false; }
}

#endif //STATIC_ALLOCATOR_STATIC_ALLOCATOR_HPP

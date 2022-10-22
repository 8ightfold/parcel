#ifndef STATIC_ALLOCATOR_STATIC_ALLOCATOR_HPP
#define STATIC_ALLOCATOR_STATIC_ALLOCATOR_HPP

#include <memory>
#include "heap.hpp"
#include "util/cttypeid.hpp"

namespace parcel {
    template <typename T>
    struct allocator {
        using value_type = T;
        using size_type = parcel::umax_t;
        using difference_type = std::ptrdiff_t;

        allocator() = default;

        template <class U>
        constexpr
        allocator(const allocator<U>&) noexcept {}

        [[nodiscard]]
        value_type* allocate(size_type n) noexcept {
            pretty_assert(n < (static_cast<std::size_t>(-1) / sizeof(value_type)),
                "parcel::allocator<T>::allocate(umax_t n):"
                " 'n' exceeds maximum supported size.");

            void* data = allocate_block(type_wrapper<T[]>{}, n);
            return launder_cast<value_type*>(data);
        }

        void deallocate(value_type* addr, size_type n) {
            deallocate_block(addr);
        }
    };

    template<class T, class U>
    bool operator==(const allocator <T>&, const allocator <U>&) { return true; }

    template<class T, class U>
    bool operator!=(const allocator <T>&, const allocator <U>&) { return false; }
}

#endif //STATIC_ALLOCATOR_STATIC_ALLOCATOR_HPP

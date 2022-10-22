#ifndef STATIC_ALLOCATOR_LAUNDER_CAST_HPP
#define STATIC_ALLOCATOR_LAUNDER_CAST_HPP

#include <new>
#include <type_traits>

namespace parcel {
    template <typename T>
    struct remove_all_pointers {
        using type = T;
    };

    template <typename T>
    struct remove_all_pointers<T*> {
        using type = typename remove_all_pointers<std::remove_pointer_t<T>>::type;
    };

    template <typename T>
    using remove_all_pointers_t = typename remove_all_pointers<T>::type;

    template <typename To, typename From>
    [[nodiscard]]
    constexpr
    To launder_cast(From* from) noexcept {
        static_assert(std::is_pointer_v<To>,
            "parcel::launder_cast<To, From>(From* from):"
            " 'To' must be a pointer type.");

        using base_t = remove_all_pointers_t<To>;
        static_assert(not std::is_void_v<base_t>,
            "parcel::launder_cast<To, From>(From* from):"
            " 'To' cannot contain type 'void'.");

        return std::launder( reinterpret_cast<To>(from) );
    }
}

#endif //STATIC_ALLOCATOR_LAUNDER_CAST_HPP

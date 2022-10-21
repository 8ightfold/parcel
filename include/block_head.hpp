#ifndef STATIC_ALLOCATOR_BLOCK_HEAD_HPP
#define STATIC_ALLOCATOR_BLOCK_HEAD_HPP

#ifndef ALLOCATOR_BLOCK_POWER
#  define ALLOCATOR_BLOCK_POWER 15
#endif

#include <new>
#include "detail/blocks.hpp"
#include "util/passert.h"

namespace parcel {
    using block_t = detail::base_block<ALLOCATOR_BLOCK_POWER>;

    [[nodiscard]]
    block_t&
    get_block_head() {
        static block_t head{};
        return head;
    }

    struct head_container {
        static inline block_t& head = get_block_head();
        static inline parcel::byte_t* start_addr = head.begin();
    };


    template <typename T>
    void* allocate_block(type_wrapper<T>) {
        static_assert(std::is_same_v<void, void>, "FIX");
        return nullptr;
    }

    template <typename T>
    void* allocate_block(type_wrapper<T[]>, parcel::umax_t n) {
        parcel::umax_t size = sizeof(T) * n;
        parcel::umax_t padding = detail::pad_addr(size);

        pretty_assert(head_container::start_addr + size + padding + sizeof(detail::header_t) + sizeof(detail::footer_t)
            > head_container::head.end(),
            "parcel::allocate_to_block<T>(type_wrapper<T[]>, parcel::umax_t n):"
            " no space remaining.");

        auto* new_header =
                new(head_container::start_addr)
                (detail::header_t)
                {{ .size = size, .padding = padding, .is_array = true, .is_active = true }};
        void* data = operator new[](n, std::launder(reinterpret_cast<parcel::byte_t*>(++new_header)));
        auto* new_footer =
                new(std::launder(reinterpret_cast<parcel::byte_t*>(data) + size + padding))
                (detail::footer_t)
                {{ .size = size, .padding = padding, .is_array = true, .is_active = true }};

        head_container::start_addr = std::launder(reinterpret_cast<parcel::byte_t*>(++new_footer));
        return data;
    }

    template <typename T, parcel::umax_t N>
    void* allocate_block(type_wrapper<T[N]>, parcel::umax_t n) {

    }

    template <typename T>
    void deallocate_block(type_wrapper<T[]>, void* addr, parcel::umax_t n) {
        auto* header = std::launder(reinterpret_cast<detail::header_t*>(
                reinterpret_cast<parcel::byte_t*>(addr) - sizeof(detail::header_t) ));
        header->data.is_active = false;

        parcel::umax_t offset = header->data.size + header->data.padding;
        auto* footer = std::launder(reinterpret_cast<detail::footer_t*>(
                reinterpret_cast<parcel::byte_t*>(addr) + offset));
        footer->data.is_active = false;

        //operator delete[] (addr, n);
    }
}

#endif //STATIC_ALLOCATOR_BLOCK_HEAD_HPP

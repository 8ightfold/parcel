#ifndef STATIC_ALLOCATOR_BLOCKS_HPP
#define STATIC_ALLOCATOR_BLOCKS_HPP

#include <cstring>
#include <new>

#include "compiler_macros.h"
#include "types.hpp"
#include "../util/launder_cast.hpp"
#include "../util/passert.h"
#include "../util/resize.hpp"

namespace parcel::detail {
    using header_size_t = umax_t;

    static constexpr
    umax_t pad_amount() {
        constexpr umax_t count =
                sizeof(header_size_t) + sizeof(void*) + sizeof(void*);
        constexpr umax_t pad_by = count % alignof(std::max_align_t);
        return pad_by;
    }

    constexpr umax_t pad_v = pad_amount();

    struct unpadded_header {
        header_size_t size : sizeof(header_size_t) * CHAR_BIT - 1 = 0;
        bool is_active : 1 = false;
        unpadded_header* prev = nullptr;
        unpadded_header* next = nullptr;
    };

    struct padded_header {
        header_size_t size : sizeof(header_size_t) * CHAR_BIT - 1 = 0;
        bool is_active : 1 = false;
        padded_header* prev = nullptr;
        padded_header* next = nullptr;
        unsigned char padding[pad_v];
    };

    using header_impl = std::conditional_t<pad_v == 0, unpadded_header, padded_header>;



    template <umax_t Blocks>
    struct global_container_t {
        static constexpr umax_t bytes = util::power_of_2<Blocks>;
        static_assert(bytes > sizeof(header_impl));

        alignas(std::max_align_t) byte_t data[bytes] { 0 };
        header_impl* heap_base;
        umax_t heap_size = 0;

        global_container_t() {
            auto* primary_header = std::launder( new(begin())(header_impl) {} );
            primary_header->prev = primary_header;
            primary_header->next = primary_header;

            heap_base = primary_header;
            heap_size = sizeof(header_impl) + heap_base->size;
        }

        constexpr
        byte_t* begin() {
            return data;
        }

        constexpr
        byte_t* end() {
            return data + bytes;
        }

        void reset_container() {
            std::memset(data, 0x0, bytes);

            auto* primary_header = std::launder( new (begin())(header_impl) {} );
            primary_header->prev = primary_header;
            primary_header->next = primary_header;

            heap_base = primary_header;
            heap_size = sizeof(header_impl) + heap_base->size;
        }

        [[nodiscard]]
        void* sbrk(umax_t n) {
            pretty_assert(data + heap_size + n < end(),
                "void* sbrk(umax_t n):"
                " insufficient heap space");

            byte_t* current_heap = data + heap_size;
            heap_size += n;
            return current_heap;
        }

        bool is_valid(void* ptr) {
            auto* arith_ptr = launder_cast<byte_t*>(ptr);
            return static_cast<bool>(arith_ptr) && (arith_ptr >= begin()) && (arith_ptr < end());
        }
    };

    void* align_addr(void* addr) {
        auto iaddr = reinterpret_cast<std::uintptr_t>(addr);
        iaddr = iaddr - (iaddr % alignof(std::max_align_t)) + alignof(std::max_align_t);
        return reinterpret_cast<void*>(iaddr);
    }

    std::uintptr_t pad_addr(std::uintptr_t addr) {
        addr = alignof(std::max_align_t) - (addr % alignof(std::max_align_t));
        return addr;
    }
}

#endif //STATIC_ALLOCATOR_BLOCKS_HPP

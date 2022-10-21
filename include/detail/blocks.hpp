#ifndef STATIC_ALLOCATOR_BLOCKS_HPP
#define STATIC_ALLOCATOR_BLOCKS_HPP

#include "compiler_macros.h"
#include "types.hpp"
#include "../util/resize.hpp"

namespace parcel::detail {
    struct block_data {
        static constexpr parcel::umax_t total_size = alignof(std::max_align_t);
        parcel::umax_t size : util::to_bits<total_size> - alignof(std::max_align_t) - 2;
        parcel::umax_t padding : alignof(std::max_align_t);
        bool is_array : 1;
        bool is_active : 1;
    };

    struct header_t {
        alignas(std::max_align_t) block_data data {};
    };

    struct footer_t {
        alignas(std::max_align_t) block_data data {};
    };

    template <parcel::umax_t Blocks>
    struct base_block {
        alignas(std::max_align_t) parcel::byte_t data[Blocks] { 0 };

        constexpr
        parcel::byte_t* begin() {
            return data;
        }

        constexpr
        parcel::byte_t* end() {
            return data + Blocks;
        }

        bool is_valid(void* ptr) {
            auto* arith_ptr = reinterpret_cast<parcel::byte_t*>(ptr);
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

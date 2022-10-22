#ifndef STATIC_ALLOCATOR_HEAP_HPP
#define STATIC_ALLOCATOR_HEAP_HPP

#ifndef ALLOCATOR_BLOCK_POWER
#  define ALLOCATOR_BLOCK_POWER 16
#endif

#include "detail/blocks.hpp"
#include "util/passert.h"

namespace parcel {
    using heap_t = detail::global_container_t<ALLOCATOR_BLOCK_POWER>;
    using header_t = detail::header_impl;

    [[nodiscard]]
    heap_t&
    get_heap() {
        static heap_t heap{};
        return heap;
    }

    struct heap {
        static inline heap_t& data = get_heap();
    };


    header_t* find_fit(umax_t n) {
        header_t* node = heap::data.heap_base;
        for(; node->next != heap::data.heap_base; node = node->next) {
            if(node->size >= n) {
                // remove node from list
                node->prev->next = node->next;
                node->next->prev = node->prev;
                node->is_active = true;
                return node;
            }
        }
        return nullptr;
    }


    template <typename T>
    void* allocate_block(type_wrapper<T>) {
        umax_t size = sizeof(T);
        umax_t padding = detail::pad_addr(size);

        header_t* header = find_fit(size);
        if(header) {
            return launder_cast<byte_t*>(++header);
        }
        else {
            auto* raw_data = launder_cast<byte_t*>(heap::data.sbrk(sizeof(header_t) + size + padding));
            auto* new_header = std::launder( new(raw_data) (header_t) { .size = size + padding, .is_active = true } );
            return launder_cast<byte_t*>(++new_header);
        }
    }

    template <typename T>
    void* allocate_block(type_wrapper<T[]>, umax_t n) {
        umax_t size = sizeof(T) * n;
        umax_t padding = detail::pad_addr(size);

        header_t* header = find_fit(size);
        if(header) {
            return launder_cast<byte_t*>(++header);
        }
        else {
            auto* raw_data = launder_cast<byte_t*>(heap::data.sbrk(sizeof(header_t) + size + padding));
            auto* new_header = std::launder( new(raw_data) (header_t) { .size = size + padding, .is_active = true} );
            return launder_cast<byte_t*>(++new_header);
        }
    }

    void deallocate_block(void* addr) {
        pretty_assert(heap::data.is_valid(addr),
            "void deallocate_block(void* addr):"
            " 'addr' is not part of the current heap");

        auto* block_header = launder_cast<header_t*>(reinterpret_cast<byte_t*>(addr) - sizeof(header_t));
        pretty_assert(block_header->is_active,
            "void deallocate_block(void* addr):"
            " double free detected");

        block_header->is_active = false;
        auto* list_head = heap::data.heap_base;

        block_header->prev = list_head;
        block_header->next = list_head->next;
        list_head->next->prev = block_header;
        list_head->next = block_header;
    }
}

#endif //STATIC_ALLOCATOR_HEAP_HPP

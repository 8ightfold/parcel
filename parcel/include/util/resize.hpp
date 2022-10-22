#ifndef STATIC_ALLOCATOR_RESIZE_HPP
#define STATIC_ALLOCATOR_RESIZE_HPP

#include <climits>
#include <utility>
#include "../detail/types.hpp"

namespace parcel::util {


    template <parcel::umax_t N>
    static constexpr
    parcel::umax_t fold_power() {
        if constexpr( N == 0 ) {
            return 1;
        }
        else if constexpr( N == 1 ) {
            return 2;
        }
        else {
            return [] <parcel::umax_t...NN> (parcel::seq_t<NN...>) {
                return 2 * (... * (NN, 2));
            } (parcel::make_seq_t<N - 1>{});
        }
    }

    template <parcel::umax_t N>
    constexpr
    parcel::umax_t power_of_2 = fold_power<N>();

    template <parcel::umax_t Bytes>
    constexpr
    parcel::umax_t to_bits = Bytes * CHAR_BIT;
}


#endif //STATIC_ALLOCATOR_RESIZE_HPP

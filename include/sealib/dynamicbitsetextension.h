//
// Created by jmeintrup on 02.05.18.
//

#ifndef SEA_DYNAMICBITSETEXTENSION_H
#define SEA_DYNAMICBITSETEXTENSION_H

#include <boost/dynamic_bitset.hpp>

template <typename Block, typename Allocator>
unsigned long block_index(const boost::dynamic_bitset<Block, Allocator>& b, unsigned long pos)
{ return pos / b.bits_per_block; }

namespace boost {
    template <>
    inline void
    to_block_range(const dynamic_bitset<>& b, std::tuple<unsigned long, unsigned long,  unsigned long&> param)
    {
        {
            unsigned long beg = std::get<0>(param);
            unsigned long len = std::get<1>(param);
            unsigned long block1 = block_index(b, beg);
            unsigned long block2 = block_index(b, beg + len -1);
            unsigned long bit_index = beg % b.bits_per_block;
            unsigned long bitmask = (1ul << len) - 1;
            std::get<2>(param) = ((b.m_bits[block1] >> bit_index) |
                             (b.m_bits[block2] << (b.bits_per_block - bit_index)  )) &
                            bitmask;
            return;
        }
    }
}
#endif //SEA_DYNAMICBITSETEXTENSION_H

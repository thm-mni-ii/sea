#include <sealib/collection/bitset.h>
#include <iostream>

using Sealib::Bitset;

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>::Bitset(uint64_t bits_) :
    bits(bits_),
    mbits(bits % bitsPerBlock == 0 ? bits / bitsPerBlock : bits / bitsPerBlock + 1) {}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>::Bitset(std::vector<bool> &&bitvector) :
    bits(static_cast<uint64_t>(bitvector.size())),
    mbits(bits % bitsPerBlock == 0 ? bits / bitsPerBlock : bits / bitsPerBlock + 1) {
    uint64_t index = 0;
        for (bool a : bitvector) {
            operator[](index) = a;
            index++;
        }
    }

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>::Bitset() : Bitset(0) {}

template<typename BlockType, typename AllocatorType>
typename Bitset<BlockType, AllocatorType>::BitReference
Bitset<BlockType, AllocatorType>::operator[](uint64_t bit) {
    assert(bit < bits);
    return BitReference(&mbits[bit / bitsPerBlock], bit % bitsPerBlock);
}

template<typename BlockType, typename AllocatorType>
bool Bitset<BlockType, AllocatorType>::operator[](uint64_t bit) const { return get(bit); }

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::set() {
    for (BlockType &mbit : mbits) {
        mbit = std::numeric_limits<BlockType>::max();
    }
}

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::clear() {
    for (BlockType &mbit : mbits) {
        mbit = 0;
    }
}

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::flip() {
    for (uint64_t i = 0; i < blocks(); ++i) {
        mbits[i] = static_cast<BlockType>(~mbits[i]);
    }
}

template<typename BlockType, typename AllocatorType>
bool Bitset<BlockType, AllocatorType>::get(uint64_t bit) const {
    assert(bit < bits);
    return get(mbits[bit / bitsPerBlock], bit % bitsPerBlock);
}

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::insert(uint64_t index, bool value) {
    operator[](index) = value;
}

template<typename BlockType, typename AllocatorType>
const BlockType &Bitset<BlockType, AllocatorType>::getBlock(uint64_t idx) const {
    assert(mbits.size() > idx);
    return mbits[idx];
}

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::setBlock(uint64_t idx, BlockType block) {
    assert(mbits.size() > idx);
    mbits[idx] = block;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>
&Bitset<BlockType, AllocatorType>::operator&=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (uint64_t i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>&
Bitset<BlockType, AllocatorType>::operator|=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (uint64_t i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] | rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>&
Bitset<BlockType, AllocatorType>::operator^=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (uint64_t i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] ^ rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>&
Bitset<BlockType, AllocatorType>::operator-=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (uint64_t i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & static_cast<BlockType>(~rhs.mbits[i]);
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType> Bitset<BlockType, AllocatorType>::operator~() const {
    Bitset b(*this);
    b.flip();
    return b;
}

template<typename BlockType, typename AllocatorType>
BlockType Sealib::Bitset<BlockType, AllocatorType>::getShiftedBlock(uint64_t idx) const {
    BlockType len = Sealib::Bitset<BlockType>::bitsPerBlock;
    BlockType b1 = mbits[idx/len];
    BlockType b2 = mbits[(idx+len-1)/len];

    BlockType bitIdx = static_cast<BlockType>(idx % len);

    return static_cast<BlockType>((b1 >> bitIdx) | (b2 << (len - bitIdx)));
}

namespace Sealib {

template
class Bitset<uint64_t, std::allocator<uint64_t>>;
template
class Bitset<uint32_t, std::allocator<uint32_t>>;
template
class Bitset<uint16_t, std::allocator<uint16_t>>;
template
class Bitset<uint8_t, std::allocator<uint8_t>>;
}  // namespace Sealib

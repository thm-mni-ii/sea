#include <sealib/collection/bitset.h>
#include <iostream>

using Sealib::Bitset;

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>::Bitset(sizeType bits_) :
    bits(bits_),
    mbits(bits % bitsPerBlock == 0 ? bits / bitsPerBlock : bits / bitsPerBlock + 1) {}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>::Bitset(const std::vector<bool> &bitvector) :
    bits(bitvector.size()),
    mbits(bits % bitsPerBlock == 0 ? bits / bitsPerBlock : bits / bitsPerBlock + 1) {
    sizeType index = 0;
        for (bool a : bitvector) {
            operator[](index) = a;
            index++;
        }
    }

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>::Bitset() : Bitset(0) {}

template<typename BlockType, typename AllocatorType>
typename Bitset<BlockType, AllocatorType>::BitReference
Bitset<BlockType, AllocatorType>::operator[](sizeType bit) {
    assert(bit < bits);
    return BitReference(&mbits[bit / bitsPerBlock], bit % bitsPerBlock);
}

template<typename BlockType, typename AllocatorType>
bool Bitset<BlockType, AllocatorType>::operator[](sizeType bit) const { return get(bit); }

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
    for (sizeType i = 0; i < blocks(); ++i) {
        mbits[i] = static_cast<BlockType>(~mbits[i]);
    }
}

template<typename BlockType, typename AllocatorType>
typename Bitset<BlockType, AllocatorType>::bitType
Bitset<BlockType, AllocatorType>::get(sizeType bit) const {
    assert(bit < bits);
    return get(mbits[bit / bitsPerBlock], bit % bitsPerBlock);
}

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::insert(sizeType index, bitType value) {
    operator[](index) = value;
}

template<typename BlockType, typename AllocatorType>
typename Bitset<BlockType, AllocatorType>::sizeType
Bitset<BlockType, AllocatorType>::size() const {
    return bits;
}

template<typename BlockType, typename AllocatorType>
typename Bitset<BlockType, AllocatorType>::sizeType
Bitset<BlockType, AllocatorType>::blocks() const {
    return mbits.size();
}

template<typename BlockType, typename AllocatorType>
const BlockType &Bitset<BlockType, AllocatorType>::getBlock(sizeType idx) const {
    assert(mbits.size() > idx);
    return mbits[idx];
}

template<typename BlockType, typename AllocatorType>
void Bitset<BlockType, AllocatorType>::setBlock(sizeType idx, BlockType block) {
    assert(mbits.size() > idx);
    mbits[idx] = block;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>
&Bitset<BlockType, AllocatorType>::operator&=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>&
Bitset<BlockType, AllocatorType>::operator|=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] | rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>&
Bitset<BlockType, AllocatorType>::operator^=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] ^ rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType, typename AllocatorType>
Bitset<BlockType, AllocatorType>&
Bitset<BlockType, AllocatorType>::operator-=(const Bitset<BlockType, AllocatorType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
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
BlockType
Sealib::Bitset<BlockType, AllocatorType>::getShiftedBlock(Bitset::sizeType idx) const {
    BlockType len = Sealib::Bitset<BlockType>::bitsPerBlock;
    BlockType b1 = mbits[idx/len];
    BlockType b2 = mbits[(idx+len-1)/len];

    BlockType bitIdx = idx % len;

    return (b1 >> bitIdx | b2 << (len - bitIdx));
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

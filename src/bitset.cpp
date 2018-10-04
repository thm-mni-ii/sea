#include <sealib/bitset.h>
#include <iostream>

using Sealib::Bitset;

template<typename BlockType>
Bitset<BlockType>::Bitset(sizeType bits_) :
    bits(bits_),
    mbits(bits % bitsPerBlock == 0 ? bits / bitsPerBlock : bits / bitsPerBlock + 1) {}

template<typename BlockType>
Bitset<BlockType>::Bitset() : Bitset(0) {}

template<typename BlockType>
Bitset<BlockType>::~Bitset() {}

template<typename BlockType>
typename Bitset<BlockType>::BitReference Bitset<BlockType>::operator[](sizeType bit) {
    assert(bit < bits);
    return BitReference(&mbits[bit / bitsPerBlock], bit % bitsPerBlock);
}

template<typename BlockType>
bool Bitset<BlockType>::operator[](sizeType bit) const { return get(bit); }

template<typename BlockType>
void Bitset<BlockType>::set() {
    for (BlockType &mbit : mbits) {
        mbit = std::numeric_limits<BlockType>::max();
    }
}

template<typename BlockType>
void Bitset<BlockType>::clear() {
    for (BlockType &mbit : mbits) {
        mbit = 0;
    }
}

template<typename BlockType>
void Bitset<BlockType>::flip() {
    for (sizeType i = 0; i < blocks(); ++i) {
        mbits[i] = static_cast<BlockType>(~mbits[i]);
    }
}

template<typename BlockType>
typename Bitset<BlockType>::bitType Bitset<BlockType>::get(sizeType bit) const {
    assert(bit < bits);
    return get(mbits[bit / bitsPerBlock], bit % bitsPerBlock);
}

template<typename BlockType>
typename Bitset<BlockType>::sizeType Bitset<BlockType>::size() const {
    return bits;
}

template<typename BlockType>
void Bitset<BlockType>::resize(unsigned long size) {
    bits = size;
    mbits.resize((size / bitsPerBlock) + 1);
}

template<typename BlockType>
typename Bitset<BlockType>::sizeType Bitset<BlockType>::blocks() const {
    return mbits.size();
}

template<typename BlockType>
const BlockType &Bitset<BlockType>::getBlock(sizeType idx) const {
    assert(mbits.size() > idx);
    return mbits[idx];
}

template<typename BlockType>
void Bitset<BlockType>::setBlock(sizeType idx, BlockType block) {
    assert(mbits.size() > idx);
    mbits[idx] = block;
}

template<typename BlockType>
Bitset<BlockType> &Bitset<BlockType>::operator&=(const Bitset<BlockType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType>
Bitset<BlockType> &Bitset<BlockType>::operator|=(const Bitset<BlockType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] | rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType>
Bitset<BlockType> &Bitset<BlockType>::operator^=(const Bitset<BlockType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] ^ rhs.mbits[i];
    }
    return *this;
}

template<typename BlockType>
Bitset<BlockType> &Bitset<BlockType>::operator-=(const Bitset<BlockType> &rhs) {
    assert(size() == rhs.size());
    for (sizeType i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & static_cast<BlockType>(~rhs.mbits[i]);
    }
    return *this;
}

template<typename BlockType>
Bitset<BlockType> Bitset<BlockType>::operator~() const {
    Bitset b(*this);
    b.flip();
    return b;
}

template<typename BlockType>
BlockType Sealib::Bitset<BlockType>::getShiftedBlock(Bitset::sizeType idx) const {
    BlockType len = Sealib::Bitset<BlockType>::bitsPerBlock;
    BlockType b1 = mbits[idx/len];
    BlockType b2 = mbits[(idx+len-1)/len];

    BlockType bitIdx = idx % len;
    BlockType bitMask = (BlockType(1) << len) - BlockType(1);

    return (b1 >> bitIdx | b2 << (len - bitIdx)) & bitMask;
}

namespace Sealib {

template
class Bitset<unsigned long>;
template
class Bitset<unsigned int>;
template
class Bitset<unsigned short>;
template
class Bitset<unsigned char>;
}  // namespace Sealib

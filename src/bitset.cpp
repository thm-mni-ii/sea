#include <sealib/bitset.h>

using Sealib::Bitset;

template<typename Block>
Bitset<Block>::Bitset(sizetype bits_) :
    bits(bits_),
    mbits(bits % bitsPerByte == 0 ? bits / bitsPerByte : bits / bitsPerByte + 1) {}

template<typename Block>
Bitset<Block>::Bitset() : Bitset(0) {}

template<typename Block>
Bitset<Block>::~Bitset() {}

template<typename Block>
typename Bitset<Block>::BitReference Bitset<Block>::operator[](sizetype bit) {
    assert(bit < bits);
    return BitReference(&mbits[bit / bitsPerByte], bit % bitsPerByte);
}

template<typename Block>
bool Bitset<Block>::operator[](sizetype bit) const { return get(bit); }

template<typename Block>
void Bitset<Block>::set() {
    for (Block &mbit : mbits) {
        mbit = std::numeric_limits<Block>::max();
    }
}

template<typename Block>
void Bitset<Block>::clear() {
    for (Block &mbit : mbits) {
        mbit = 0;
    }
}

template<typename Block>
typename Bitset<Block>::bittype Bitset<Block>::get(sizetype bit) const {
    assert(bit < bits);
    return get(mbits[bit / bitsPerByte], bit % bitsPerByte);
}

template<typename Block>
typename Bitset<Block>::sizetype Bitset<Block>::size() const {
    return bits;
}

template<typename Block>
typename Bitset<Block>::sizetype Bitset<Block>::blocks() const {
    return mbits.size();
}

template<typename Block>
const Block &Bitset<Block>::getBlock(sizetype idx) const {
    assert(mbits.size() > idx);
    return mbits[idx];
}

template<typename Block>
void Bitset<Block>::setBlock(sizetype idx, Block block) {
    assert(mbits.size() > idx);
    mbits[idx] = block;
}

template<typename Block>
Bitset<Block> &Bitset<Block>::operator&=(const Bitset<Block> &rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & rhs.mbits[i];
    }
    return *this;
}

template<typename Block>
Bitset<Block> &Bitset<Block>::operator|=(const Bitset<Block> &rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] | rhs.mbits[i];
    }
    return *this;
}

template<typename Block>
Bitset<Block> &Bitset<Block>::operator^=(const Bitset<Block> &rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] ^ rhs.mbits[i];
    }
    return *this;
}

template<typename Block>
Bitset<Block> &Bitset<Block>::operator-=(const Bitset<Block> &rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] = mbits[i] & static_cast<Block>(~rhs.mbits[i]);
    }
    return *this;
}
namespace Sealib {
template<typename Block>
Bitset<Block> operator&(const Bitset<Block> &lhs, const Bitset<Block> &rhs) {
    Bitset<Block> b(lhs);
    return b &= rhs;
}

template<typename Block>
Bitset<Block> operator|(const Bitset<Block> &lhs, const Bitset<Block> &rhs) {
    Bitset<Block> b(lhs);
    return b |= rhs;
}

template<typename Block>
Bitset<Block> operator^(const Bitset<Block> &lhs, const Bitset<Block> &rhs) {
    Bitset<Block> b(lhs);
    return b ^= rhs;
}

template<typename Block>
Bitset<Block> operator-(const Bitset<Block> &lhs, const Bitset<Block> &rhs) {
    Bitset<Block> b(lhs);
    return b -= rhs;
}

template
class Bitset<unsigned long>;
template
class Bitset<unsigned int>;
template
class Bitset<unsigned short>;
template
class Bitset<unsigned char>;
}  // namespace Sealib

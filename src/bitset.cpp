#include <sealib/bitset.h>

using Sealib::Bitset;

Bitset::Bitset(sizetype bits_) :
        bits(bits_),
        mbits(bits % bitsPerByte == 0 ? bits/bitsPerByte : bits/bitsPerByte + 1) {}

Bitset::Bitset() : Bitset(0) {}

Bitset::~Bitset() {}

Bitset::BitReference Bitset::operator[](sizetype bit) {
    assert(bit < bits);
    return BitReference(&mbits[bit / bitsPerByte], bit % bitsPerByte);
}

bool Bitset::operator[](sizetype bit) const { return Bitset::get(bit); }

void Bitset::set() {
    for (unsigned long &mbit : mbits) {
        mbit = std::numeric_limits<blocktype>::max();
    }
}

void Bitset::clear() {
    for (unsigned long &mbit : mbits) {
        mbit = 0;
    }
}

Bitset::bittype Bitset::get(sizetype bit) const {
    assert(bit < bits);
    return get(mbits[bit / bitsPerByte], bit % bitsPerByte);
}

Bitset::sizetype Bitset::size() const {
    return bits;
}

Bitset::sizetype Bitset::blocks() const {
    return mbits.size();
}

const Bitset::blocktype& Bitset::getBlock(sizetype idx) const {
    assert(mbits.size() > idx);
    return mbits[idx];
}

void Bitset::setBlock(sizetype idx,  blocktype block) {
    assert(mbits.size() > idx);
    mbits[idx] = block;
}

Bitset& Bitset::operator&=(const Bitset& rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] &= rhs.mbits[i];
    }
    return *this;
}

Bitset& Bitset::operator|=(const Bitset& rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] |= rhs.mbits[i];
    }
    return *this;
}

Bitset& Bitset::operator^=(const Bitset& rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] ^= rhs.mbits[i];
    }
    return *this;
}

Bitset& Bitset::operator-=(const Bitset& rhs) {
    assert(size() == rhs.size());
    for (sizetype i = 0; i < blocks(); i++) {
        mbits[i] &= ~rhs.mbits[i];
    }
    return *this;
}
namespace Sealib {
Bitset operator&(const Bitset& lhs, const Bitset& rhs) {
    Bitset b(lhs);
    return b &= rhs;
}

Bitset operator|(const Bitset& lhs, const Bitset& rhs) {
    Bitset b(lhs);
    return b |= rhs;
}

Bitset operator^(const Bitset& lhs, const Bitset& rhs) {
    Bitset b(lhs);
    return b ^= rhs;
}

Bitset operator-(const Bitset& lhs, const Bitset& rhs) {
    Bitset b(lhs);
    return b -= rhs;
}

bool operator==(const Bitset& lhs, const Bitset& rhs) {
    return (lhs.size() == rhs.size()) && (lhs.mbits == rhs.mbits);
}
}  // namespace Sealib

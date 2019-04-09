#include "sealib/collection/variantbitset.h"

namespace Sealib {

uint8_t VariantBitset::getShiftedBlock(uint64_t i) const {
    uint8_t len = 32;
    uint8_t b1 = bit[i / len];
    uint8_t b2 = bit[(i + len - 1) / len];
    uint8_t bitIdx = static_cast<uint8_t>(i % len);
    return static_cast<uint8_t>((b1 >> bitIdx) | (b2 << (len - bitIdx)));
}

VariantBitset::VariantBitset(VariantBitset const &p) : bit(p.bit) {}

VariantBitset::VariantBitset(VariantBitset &&p) : bit(std::move(p.bit)) {}

VariantBitset &VariantBitset::operator=(VariantBitset const &p) {
    bit = p.bit;
    return *this;
}

VariantBitset &VariantBitset::operator=(VariantBitset &&p) {
    bit = std::move(p.bit);
    return *this;
}

VariantBitset::~VariantBitset() { bit.~vector(); }

}  // namespace Sealib

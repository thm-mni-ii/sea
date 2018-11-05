#include <sealib/dyckmatchingstructure.h>
#include <iostream>

uint64_t Sealib::DyckMatchingStructure::getMatchNaive(
    const Sealib::Bitset<uint8_t> &word,
    uint64_t idx) {
    uint32_t j = 0;
    uint32_t p = 0;
    std::vector<uint32_t> stack(word.size());
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            if (p == 0) {
                return idx;
            }
            uint32_t i = stack[--p];
            if (idx == i) {
                return j;
            }
            if (idx == j) {
                return i;
            }
        }
        j++;
    } while (j != word.size());

    return idx;;
}

const Sealib::Bitset<uint8_t> &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const Sealib::Bitset<uint8_t> &word_) :
    word(word_) {}

Sealib::DyckMatchingStructure::~DyckMatchingStructure() {}

uint64_t Sealib::DyckMatchingStructure::getMatch(uint64_t idx) {
    return getMatchNaive(word, idx);
}

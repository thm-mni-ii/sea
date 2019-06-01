#include "naivedyckmatchingstructure.h"

Sealib::NaiveDyckMatchingStructure::NaiveDyckMatchingStructure(
    const Sealib::Bitset<uint8_t>& word_)
    : DyckMatchingStructure(word_), table(initializeTable(word_)) {}

uint64_t Sealib::NaiveDyckMatchingStructure::getMatch(uint64_t idx) {
    return DyckMatchingStructure::getMatch(idx);
}

std::vector<uint32_t> Sealib::NaiveDyckMatchingStructure::initializeTable(
    const Sealib::Bitset<uint8_t>& word_) {
    std::vector<uint32_t> _table(word_.size());

    uint32_t j = 0;
    uint32_t p = 0;
    std::vector<uint32_t> stack(word.size());
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            uint32_t i = stack[--p];
            _table[j] = i;
            _table[i] = j;
        }
        j++;
    } while (j != word.size());

    return _table;
}

#include <sealib/dyckmatchingstructure.h>
#include <iostream>

template<typename BlockType>
unsigned long Sealib::DyckMatchingStructure<BlockType>::getMatchNaive(
    const Sealib::Bitset<BlockType> &word,
    unsigned long idx) {
    unsigned int j = 0;
    unsigned int p = 0;
    std::vector<unsigned int> stack(word.size());
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            if (p == 0) {
                return idx;
            }
            unsigned int i = stack[--p];
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
template<typename BlockType>

const Sealib::Bitset<BlockType> &Sealib::DyckMatchingStructure<BlockType>::getWord() const {
    return word;
}
template<typename BlockType>

Sealib::DyckMatchingStructure<BlockType>::DyckMatchingStructure(
    const Sealib::Bitset<BlockType> &word_) :
    word(word_) {}
template<typename BlockType>

Sealib::DyckMatchingStructure<BlockType>::~DyckMatchingStructure() {}
template<typename BlockType>

unsigned long Sealib::DyckMatchingStructure<BlockType>::getMatch(unsigned long idx) {
    return getMatchNaive(word, idx);
}
namespace Sealib {
template
class DyckMatchingStructure<unsigned char>;
template
class DyckMatchingStructure<unsigned short>;
}  // namespace Sealib

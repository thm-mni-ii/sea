#include <sealib/dyckmatchingstructure.h>
#include <iostream>

inline unsigned long Sealib::DyckMatchingStructure::getMatchNaive(unsigned long idx) {
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

const Sealib::Bitset<unsigned char> &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_) :
    word(word_) {}

unsigned long Sealib::DyckMatchingStructure::getMatch(unsigned long idx) {
    return getMatchNaive(idx);
}

const Sealib::Bitset<unsigned char> Sealib::DyckMatchingStructure::segmentizeWord(const Sealib::Bitset<unsigned char> &word) {
    unsigned long segments = word.size() / mSegmentLength;
    Sealib::Bitset<unsigned char> segmentizedWord(word.size() + segments);

    unsigned int off = 0;
    for(unsigned int i = 0; i < word.size(); i++) {
        if(i != 0 && i % mSegmentLength == 0) {
            off++;
        }
        segmentizedWord[i + off] = word[i];
    }

    return segmentizedWord;
}

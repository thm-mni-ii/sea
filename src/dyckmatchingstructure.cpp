#include <sealib/dyckmatchingstructure.h>
#include <iostream>

unsigned long Sealib::DyckMatchingStructure::getMatchNaive(const Sealib::Bitset<unsigned char> &word,
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

unsigned long Sealib::DyckMatchingStructure::getMatchNaive(const Sealib::Bitset<unsigned char> &word, unsigned long idx,
                                                           unsigned long skip) {
    /*std::cout << std::endl;
    for (unsigned int i = 0; i < word.size(); i++) {
        if (i == 0 || i % skip != 0) {
            if (word[i]) {
                std::cout << "(";
            } else {
                std::cout << ")";
            }
        } else {
            std::cout << "*";
        }
    }
    std::cout << std::endl;*/

    unsigned int i;
    unsigned int j = 0;
    unsigned int p = 0;
    unsigned int skips = 0;
    std::vector<unsigned int> stack(word.size());
    do {
        if (j == 0 || j % skip != 0) {
            if (word[j]) {  // '('
                stack[p++] = j - skips;
            } else {
                if (p == 0) {
                    return idx;
                }
                i = stack[--p];
                //std::cout << "i: " << i << std::endl;
                if (idx == i) {
                    return j - skips;
                }
                if (idx == j - skips) {
                    return i;
                }
            }
        } else {
            skips++;
        }
        /*for (unsigned int val : stack) {
            std::cout << val;
        }
        std::cout << std::endl;
        for (unsigned int i = 0; i < p; i++) {
            std::cout << " ";
        }
        std::cout << "^" << std::endl;*/
        j++;
    } while (j != word.size());

    return idx;
}

const Sealib::Bitset<unsigned char> &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const Sealib::Bitset<unsigned char> &word_) :
    word(word_) {}

unsigned long Sealib::DyckMatchingStructure::getMatch(unsigned long idx) {
    return getMatchNaive(word, idx);
}

const Sealib::Bitset<unsigned char> Sealib::DyckMatchingStructure::segmentizeWord(const Sealib::Bitset<unsigned char> &word) {
    unsigned long segments = word.size() / mSegmentLength;
    Sealib::Bitset<unsigned char> segmentizedWord(word.size() + segments);

    unsigned int off = 0;
    for (unsigned int i = 0; i < word.size(); i++) {
        if (i != 0 && i % mSegmentLength == 0) {
            off++;
        }
        segmentizedWord[i + off] = word[i];
    }

    return segmentizedWord;
}

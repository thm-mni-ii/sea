//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/dyckmatchingstructure.h>
#include <iostream>

inline unsigned long Sealib::DyckMatchingStructure::findMatchNaive(unsigned long idx) {
    unsigned int j = 0;
    unsigned int p = 0;
    std::vector<unsigned int> stack(word.size());
    do {
            if (word[j]) {  // '('
                stack[p++] = j;
            } else {
                if(p == 0) {
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

const boost::dynamic_bitset<unsigned char> &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const boost::dynamic_bitset<unsigned char> &word_) : word(word_){

}

unsigned long Sealib::DyckMatchingStructure::findMatch(unsigned long idx) {
    return findMatchNaive(idx);
}

void Sealib::DyckMatchingStructure::printDyckWord(const boost::dynamic_bitset<unsigned char> &word)  {
    for(unsigned long i = 0; i < word.size(); i++) {
        if(word[i]) {
            std::cout << "(";
        } else {
            std::cout << ")";
        }
    }
    std::cout << std::endl;
}

void Sealib::DyckMatchingStructure::printDyckWord(const boost::dynamic_bitset<unsigned char> &word, unsigned long breaks) {
    for(unsigned long i = 0; i < word.size(); i++) {
        if(i % breaks == 0 && i != 0) {
            std::cout << " ";
        }
        if(word[i]) {
            std::cout << "(";
        } else {
            std::cout << ")";
        }
    }
    std::cout << std::endl;
}
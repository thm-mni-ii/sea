//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/dyckmatchingstructure.h>
#include <iostream>

inline unsigned long Sealib::DyckMatchingStructure::findMatchNaive(unsigned long idx) {
    unsigned int j = 0;
    unsigned int p = 0;
    auto *stack = static_cast<unsigned int *>(malloc((sizeof(unsigned int) * word.size() / 2) + 1));
    do {
        if (word[j]) {  // '('
            stack[p++] = j;
        } else {
            unsigned int i = stack[--p];
            if (idx == i) {
                free(stack);
                return j;
            }
            if (idx == j) {
                free(stack);
                return i;
            }
        }
        j++;
    } while (j != word.size());
    free(stack);

    return idx;;
}

const boost::dynamic_bitset<> &Sealib::DyckMatchingStructure::getWord() const {
    return word;
}

Sealib::DyckMatchingStructure::DyckMatchingStructure(const boost::dynamic_bitset<> &word_) : word(word_){

}

unsigned long Sealib::DyckMatchingStructure::findMatch(unsigned long idx) {
    return findMatchNaive(idx);
}

void Sealib::DyckMatchingStructure::printDyckWord(const boost::dynamic_bitset<> &word)  {
    for(unsigned long i = 0; i < word.size(); i++) {
        if(word[i]) {
            std::cout << "(";
        } else {
            std::cout << ")";
        }
    }
    std::cout << std::endl;
}

void Sealib::DyckMatchingStructure::printDyckWord(const boost::dynamic_bitset<> &word, unsigned long breaks) {
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

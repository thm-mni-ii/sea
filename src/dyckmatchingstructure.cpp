//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/dyckmatchingstructure.h>

unsigned long DyckMatchingStructure::findMatchNaive(boost::dynamic_bitset<> &word_, unsigned long idx) {
    unsigned int j = 0;
    unsigned int p = 0;
    auto *stack = static_cast<unsigned int *>(malloc((sizeof(unsigned int) * word_.size() / 2)));
    do {
        if (word_[j]) {  // '('
            stack[p++] = j;
        } else {
            unsigned int i = stack[--p];
            if (idx == i) {
                return j;
            }
            if (idx == j) {
                return i;
            }
        }
        j++;
    } while (j != word_.size());

    return idx;;
}

const boost::dynamic_bitset<> &DyckMatchingStructure::getWord() const {
    return word;
}

DyckMatchingStructure::DyckMatchingStructure(const boost::dynamic_bitset<> &word_) : word(word_){

}

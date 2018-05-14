//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/recursivedyckmatchingstructure.h>
#include <cmath>

std::map<unsigned char, Sealib::LocalDyckTable> RecursiveDyckMatchingStructure::tables = std::map<unsigned char, Sealib::LocalDyckTable>();

const Sealib::LocalDyckTable &RecursiveDyckMatchingStructure::getOrCreateTable(unsigned char segmentSize) {
    if(RecursiveDyckMatchingStructure::tables.find(segmentSize) != RecursiveDyckMatchingStructure::tables.end()) {
        return RecursiveDyckMatchingStructure::tables[segmentSize];
    } else {
        RecursiveDyckMatchingStructure::tables.insert(std::make_pair(segmentSize, Sealib::LocalDyckTable(segmentSize)));
        return RecursiveDyckMatchingStructure::tables[segmentSize];
    };
}

unsigned long RecursiveDyckMatchingStructure::findMatchNaive(boost::dynamic_bitset<> &word_, unsigned long idx) {
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

RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word_, bool last) :
        DyckMatchingStructure(word_),
        pioneerMatchingStructure(nullptr){
    segmentLength = static_cast<unsigned char>(std::log2(word_.size()));
    boost::dynamic_bitset<> pioneerRankSelectBitSet(word_.size());
    
    initializePioneerRankSelectBitset(word_, pioneerRankSelectBitSet);

    if (last) {

    } else {
        pioneerMatchingStructure = new RecursiveDyckMatchingStructure(word_);
    }

}

RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word_) : DyckMatchingStructure(word_) {
    pioneerMatchingStructure = new DyckMatchingStructure(word_);
}

void RecursiveDyckMatchingStructure::initializePioneerRankSelectBitset(const boost::dynamic_bitset<> &word_, boost::dynamic_bitset<> &pioneerRankSelectBitset_) {
    word_[1];
    pioneerRankSelectBitset_[1] = 1;
}

//
// Created by jmeintrup on 12.05.18.
//

#include <include/sealib/recursivedyckmatchingstructure.h>
#include <cmath>

std::map<unsigned char, Sealib::LocalDyckTable> RecursiveDyckMatchingStructure::tables = std::map<unsigned char, Sealib::LocalDyckTable>();

static const Sealib::LocalDyckTable &RecursiveDyckMatchingStructure::getOrCreateTable(unsigned char segmentSize) {
    if(RecursiveDyckMatchingStructure::tables.find(segmentSize) != RecursiveDyckMatchingStructure::tables.end()) {
        return RecursiveDyckMatchingStructure::tables[segmentSize];
    } else {
        RecursiveDyckMatchingStructure::tables.insert(std::make_pair(segmentSize, Sealib::LocalDyckTable(segmentSize)));
        return RecursiveDyckMatchingStructure::tables[segmentSize];
    };
}

static const unsigned long RecursiveDyckMatchingStructure::findMatchNaive(boost::dynamic_bitset &word, unsigned long idx) {
    unsigned int j = 0;
    unsigned int p = 0;
    auto *stack = static_cast<unsigned int *>(malloc((sizeof(unsigned int) * word.size() / 2)));
    do {
        if (word[j]) {  // '('
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
    } while (j != word.size());

    return idx;;
}

RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word, bool last) :
        DyckMatchingStructure(word),
        pioneerMatchingStructure(nullptr){
    segmentLength = static_cast<unsigned char>(std::log2(word.size()));
    boost::dynamic_bitset<> pioneerRankSelectBitSet(word.size());
    
    initializePioneerRankSelectBitset(word, pioneerRankSelectBitSet);

    pioneerMatchingStructure = new RecursiveDyckMatchingStructure(word);
}

RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(const boost::dynamic_bitset<> &word) : DyckMatchingStructure(word) {
    pioneerMatchingStructure = new DyckMatchingStructure(word);
}

static void RecursiveDyckMatchingStructure::initializePioneerRankSelectBitset(const boost::dynamic_bitset &word, boost::dynamic_bitset &pioneerRankSelectBitset) {
    pioneerRankSelectBitset[1] = 1;
}

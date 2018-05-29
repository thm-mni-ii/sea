//
// Created by jmeintrup on 16.05.18.
//

#include <sealib/differencelist.h>
#include <include/sealib/rankstructure.h>
#include <iostream>

Sealib::DifferenceList::DifferenceList(unsigned long maxElements, unsigned long maxValue) :
        bits((maxValue/( (maxValue/maxElements) * (maxValue/maxElements) )) * RankStructure::log2(maxValue) + 2),
        currValue(0),
        iterationValue(0),
        iterationIdx(0),
        idx(0),
        smallVal(static_cast<unsigned char>( (maxValue / maxElements) * (maxValue / maxElements) )),
        smallBits(RankStructure::log2((maxValue / maxElements) + 2)),
        largeBits(RankStructure::log2(maxValue)),
        reverse(false){
    std::cout << (maxElements*maxElements / maxValue * RankStructure::log2(maxValue)) << std::endl;
    std::cout << (maxElements* RankStructure::log2(2 + maxValue/maxElements)) << std::endl;
    bits = boost::dynamic_bitset<unsigned char>((maxValue/( (maxValue/maxElements) * (maxValue/maxElements) )) * RankStructure::log2(maxValue) + 2);
    std::cout << bits.size() << std::endl;
}


void Sealib::DifferenceList::reset() {
    if(reverse) {
        iterationIdx = idx - 1;
    } else {
        iterationIdx = 0;
    }
    iterationValue = 0;
}

unsigned long Sealib::DifferenceList::next() {
    if(reverse) {
        if(bits[iterationIdx] == 1) {  // small value
            unsigned long val;
            boost::to_block_range(bits, std::make_tuple(iterationIdx - smallBits - 1, static_cast<unsigned long>(smallBits), std::ref(val)));
            iterationIdx -= smallBits + 2;
            iterationValue+=val;
        } else {  // large value
            unsigned long val;
            boost::to_block_range(bits, std::make_tuple(iterationIdx - largeBits - 1, static_cast<unsigned long>(largeBits), std::ref(val)));
            iterationIdx += largeBits + 1;
            iterationValue+=val;
        }
    } else {
        if(bits[iterationIdx] == 1) {  // small value
            unsigned long val;
            boost::to_block_range(bits, std::make_tuple(iterationIdx + 1, static_cast<unsigned long>(smallBits), std::ref(val)));
            iterationIdx += smallBits + 2;
            iterationValue+=val;
        } else {  // large value
            unsigned long val;
            boost::to_block_range(bits, std::make_tuple(iterationIdx + 1, static_cast<unsigned long>(largeBits), std::ref(val)));
            iterationIdx += largeBits + 2;
            iterationValue+=val;
        }
    }

    return iterationValue;
}

void Sealib::DifferenceList::setReverse() {
    reverse = true;
    reset();
}

void Sealib::DifferenceList::setForward() {
    reverse = false;
    reset();
}

void Sealib::DifferenceList::add(unsigned long val) {
    unsigned long delta = val - currValue;
    if(delta <= smallVal) {
        bits[idx++] = 1;
        for(unsigned char c = 0; c < smallBits; c++) {
            if(CHECK_BIT(val, c)) {
                bits[idx++] = 1;
            } else {
                bits[idx++] = 0;
            }
        }
        bits[idx++] = 1;
    } else {
        bits[idx++] = 0;
        for(unsigned char c = 0; c < largeBits; c++) {
            if(CHECK_BIT(val, c)) {
                bits[idx++] = 1;
            } else {
                bits[idx++] = 0;
            }
        }
        bits[idx++] = 0;
    }
    currValue = val;
}

void Sealib::DifferenceList::removeLast() {
    if(bits[idx-1] == 1) {  // small value
        idx-= smallBits - 2;
    } else {  // large value
        idx-= largeBits - 2;
    };
}

Sealib::DifferenceList::~DifferenceList() {
    bits.reset();
}

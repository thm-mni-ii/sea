//
// Created by jmeintrup on 20.03.18.
//

#include "sealib/dyckword.h"

size_t DyckWord::findMatch(size_t idx) {
    size_t p = 0;
    size_t stack[bits.size()/2];
    for(size_t j = 0; j < bits.size(); j++) {
        if (bits.at(j)) {
            stack[p++] = j;
        } else {
            size_t i = stack[--p];
            if (idx == i) return j;
            if (idx == j) return i;
        }
    }
    return idx;
}

DyckWord::DyckWord(size_t size) : bits(size){

}

void DyckWord::flip(size_t idx) {
    bits.at(idx).flip();
}

std::_Bit_reference DyckWord::at(size_t idx) {
    return bits.at(idx);
}



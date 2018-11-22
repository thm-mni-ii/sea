#include <cstdlib>
#include <cmath>
#include <iostream>
#include "sealib/localdycktable.h"

#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

Sealib::LocalDyckTable::LocalDyckTable() : table(256) {
    for (uint32_t i = 0; i < 256; i++) {
        table[i] = Sealib::LocalDyckTable::Data((uint8_t) i);
    }
}

const Sealib::LocalDyckTable::Data& Sealib::LocalDyckTable::getLocalData(uint8_t segment) {
    static LocalDyckTable instance;
    return instance.table[segment];
}

Sealib::LocalDyckTable::Data::Data() {}

Sealib::LocalDyckTable::Data::Data(uint8_t segment) :
    leftPioneer((uint8_t) - 1),
    rightPioneer((uint8_t) - 1) {
    for (uint8_t c = 0; c < kSegLen; c++) {
        localMatches[c] = c;
        localDepths[c] = 0;
    }
    uint8_t j = 0;
    uint8_t p = 0;
    std::vector<uint8_t> stack(kSegLen);
    while (j != stack.size()) {
        if (CHECK_BIT(segment, j)) {  // '('
            stack[p++] = j;
        } else {
            if (p > 0) {  // j > stack[p]
                localMatches[j] = stack[--p];
                localMatches[stack[p]] = j;
                for (auto c = static_cast<uint8_t>(stack[p] + 1); c < j; c++) {
                    localDepths[c]++;
                }
            }
        }
        j++;
    }
    char openingDepth = 0;
    char closingDepth = 0;
    for (uint8_t c = 0; c < kSegLen; c++) {
        if (localMatches[c] == c && CHECK_BIT(segment, c)) {  // global opening
            localDepths[c] = ++openingDepth;
        }
        if (localMatches[kSegLen - c - 1] == kSegLen - c - 1
            && !CHECK_BIT(segment, kSegLen - c - 1)) {  // global closing
            localDepths[kSegLen - c - 1] = --closingDepth;
        }
    }
    for (uint8_t c = 0; c < kSegLen; c++) {
        if (localMatches[c] == c && CHECK_BIT(segment, c)) {
            leftPioneer = c;
            break;
        }
    }
    for (int c = kSegLen - 1; c >= 0; c--) {
        if (localMatches[c] == c && !CHECK_BIT(segment, c)) {
            rightPioneer = static_cast<uint8_t>(c);
            break;
        }
    }
}

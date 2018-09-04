#include <cstdlib>
#include <cmath>
#include <iostream>
#include "sealib/localdycktable.h"

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

Sealib::LocalDyckTable::LocalDyckTable() : table(256) {
    for (unsigned int i = 0; i < 256; i++) {
        table[i] = Sealib::LocalDyckTable::Data((unsigned char) i);
    }
}

const Sealib::LocalDyckTable::Data& Sealib::LocalDyckTable::getLocalData(unsigned char segment) {
    static LocalDyckTable instance;
    return instance.table[segment];
}

Sealib::LocalDyckTable::Data::Data() {}

Sealib::LocalDyckTable::Data::Data(unsigned char segment) :
    leftPioneer((unsigned char) - 1),
    rightPioneer((unsigned char) - 1) {
    for (unsigned char c = 0; c < segmentLength; c++) {
        localMatches[c] = c;
        localDepths[c] = 0;
    }
    unsigned char j = 0;
    unsigned char p = 0;
    std::vector<unsigned char> stack(segmentLength);
    while (j != stack.size()) {
        if (CHECK_BIT(segment, j)) {  // '('
            stack[p++] = j;
        } else {
            if (p > 0) {  // j > stack[p]
                localMatches[j] = stack[--p];
                localMatches[stack[p]] = j;
                for (auto c = static_cast<unsigned char>(stack[p] + 1); c < j; c++) {
                    localDepths[c]++;
                }
            }
        }
        j++;
    }
    char openingDepth = 0;
    char closingDepth = 0;
    for (unsigned char c = 0; c < segmentLength; c++) {
        if (localMatches[c] == c && CHECK_BIT(segment, c)) {  // global opening
            localDepths[c] = ++openingDepth;
        }
        if (localMatches[segmentLength - c - 1] == segmentLength - c - 1 && !CHECK_BIT(segment, segmentLength - c - 1)) {  // global closing
            localDepths[segmentLength - c - 1] = --closingDepth;
        }
    }
    for (unsigned char c = 0; c < segmentLength; c++) {
        if (localMatches[c] == c && CHECK_BIT(segment, c)) {
            leftPioneer = c;
            break;
        }
    }
    for (int c = segmentLength - 1; c >= 0; c--) {
        if (localMatches[c] == c && !CHECK_BIT(segment, c)) {
            rightPioneer = static_cast<unsigned char>(c);
            break;
        }
    }
}

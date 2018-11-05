#include <cstdlib>
#include <cmath>
#include <iostream>
#include "sealib/localdycktable.h"

#define CHECK_BIT(var, pos) (((var)>>(pos)) & 1)

template <typename BlockType>
Sealib::LocalDyckTable<BlockType>::LocalDyckTable() : table(256) {
    for (unsigned int i = 0; i < 256; i++) {
        table[i] = Sealib::LocalDyckTable<BlockType>::Data((unsigned char) i);
    }
}

template <typename BlockType>
const typename Sealib::LocalDyckTable<BlockType>::Data& Sealib::LocalDyckTable<BlockType>::getLocalData(BlockType segment) {
    static LocalDyckTable<BlockType> instance;
    return instance.table[segment];
}
template<typename BlockType>
Sealib::LocalDyckTable<BlockType>::~LocalDyckTable() = default;

template <typename BlockType>
Sealib::LocalDyckTable<BlockType>::Data::Data() = default;

template <typename BlockType>
Sealib::LocalDyckTable<BlockType>::Data::Data(BlockType segment) :
    leftPioneer(blockTypeMax),
    rightPioneer(blockTypeMax) {
    for (unsigned char c = 0; c < kSegLen; c++) {
        localMatches[c] = c;
        localDepths[c] = 0;
    }
    unsigned char j = 0;
    unsigned char p = 0;
    std::vector<unsigned char> stack(kSegLen);
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
    for (unsigned char c = 0; c < kSegLen; c++) {
        if (localMatches[c] == c && CHECK_BIT(segment, c)) {  // global opening
            localDepths[c] = ++openingDepth;
        }
        if (localMatches[kSegLen - c - 1] == kSegLen - c - 1
            && !CHECK_BIT(segment, kSegLen - c - 1)) {  // global closing
            localDepths[kSegLen - c - 1] = --closingDepth;
        }
    }
    for (unsigned char c = 0; c < kSegLen; c++) {
        if (localMatches[c] == c && CHECK_BIT(segment, c)) {
            leftPioneer = c;
            break;
        }
    }
    for (int c = kSegLen - 1; c >= 0; c--) {
        if (localMatches[c] == c && !CHECK_BIT(segment, c)) {
            rightPioneer = static_cast<unsigned char>(c);
            break;
        }
    }
}


namespace Sealib {
template
class LocalDyckTable<unsigned char>;
template
class LocalDyckTable<unsigned short>;
}  // namespace Sealib

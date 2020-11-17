#include <sealib/collection/cdbitutil.h>
#include <cmath>
#include <exception>
#include <iostream>

using Sealib::CdBitUtil;

int64_t CdBitUtil::cdXorLoop(uint64_t _word, uint64_t color, uint64_t _colorFieldSize) {
    uint64_t compareField;
    uint64_t word;
    bool found = false;
    uint64_t field = 0;
    uint64_t colorFieldSize = _colorFieldSize;
    uint64_t mask = getMask(colorFieldSize);
    int64_t index = -1;

    word = foldWord(generateXorWord(_word, color, colorFieldSize), colorFieldSize);

    while (!found && field < WORDSIZE / colorFieldSize) {
        compareField = word & (mask << field * colorFieldSize);
        if (compareField & (compareField >> colorFieldSize / 2)) {
            index = (long)(WORDSIZE / colorFieldSize - field - SHIFT_OFFSET);
            found = true;
        }
        field++;
    }
    return index;
}

int64_t CdBitUtil::cdXor(uint64_t _word, uint64_t color, uint64_t colorFieldSize) {
    uint64_t word;
    int64_t index = -1;

    word = foldWord(generateXorWord(_word, color, colorFieldSize), colorFieldSize);

    if (word > 0) {
        index = __builtin_clzl(word) / (int)colorFieldSize;
    }

    return index;
}

uint64_t CdBitUtil::cdColorIndices(uint64_t word, uint64_t color,
                                            uint64_t colorFieldSize) {
    return foldWord(generateXorWord(word, color, colorFieldSize), colorFieldSize);
}

uint64_t CdBitUtil::generateXorWord(uint64_t word,
                                             uint64_t color,
                                             uint64_t colorFieldSize) {
    uint64_t xorValue;
    for (uint64_t i = 0; i < WORDSIZE / colorFieldSize; i++) {
        xorValue = xorValue << colorFieldSize;
        xorValue += color;
    }
    return ~word ^ xorValue;
}

uint64_t CdBitUtil::foldWord(uint64_t _word, uint64_t colorFieldSize) {
    uint64_t xorMask;
    uint64_t maskSize = colorFieldSize;
    uint64_t word = _word;
    while (maskSize > 1) {
        xorMask = getXor(colorFieldSize);
        word = word & (word << maskSize / 2);
        word = word & xorMask;
        maskSize = maskSize / 2;
    }
    return word;
}

uint64_t CdBitUtil::getXor(uint64_t colorFieldSize) {
    switch (colorFieldSize) {
        case THIRTYTWO:
            return XOR_32;
            break;
        case SIXTEEN:
            return XOR_16;
            break;
        case EIGHT:
            return XOR_8;
            break;
        case FOUR:
            return XOR_4;
            break;
        case TWO:
            return XOR_2;
            break;
    }
    return 0;
}

uint64_t CdBitUtil::getMask(uint64_t colorFieldSize) {
    return (uint64_t)pow(2, colorFieldSize) - 1;
}

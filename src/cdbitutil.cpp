#include "sealib/cdbitutil.h"
#include <cmath>
#include <exception>
#include <iostream>

using Sealib::CdBitUtil;

long int CdBitUtil::cdXorLoop(unsigned long int _word, unsigned long int color, unsigned long int _colorFieldSize) {
    unsigned long int compareField;
    unsigned long int word;
    bool found = false;
    unsigned long int field = 0;
    unsigned long int colorFieldSize = _colorFieldSize;
    unsigned long int mask = getMask(colorFieldSize);
    long int index = -1;

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

long int CdBitUtil::cdXor(unsigned long int _word, unsigned long int color, unsigned long int colorFieldSize) {
    unsigned long int word;
    long int index = -1;

    word = foldWord(generateXorWord(_word, color, colorFieldSize), colorFieldSize);

    if (word > 0) {
        index = __builtin_clzl(word) / (int)colorFieldSize;
    }

    return index;
}

unsigned long int CdBitUtil::cdColorIndices(unsigned long int word, unsigned long int color,
                                            unsigned long int colorFieldSize) {
    return foldWord(generateXorWord(word, color, colorFieldSize), colorFieldSize);
}

unsigned long int CdBitUtil::generateXorWord(unsigned long int word,
                                             unsigned long int color,
                                             unsigned long int colorFieldSize) {
    unsigned long int xorValue;
    for (unsigned long int i = 0; i < WORDSIZE / colorFieldSize; i++) {
        xorValue = xorValue << colorFieldSize;
        xorValue += color;
    }
    return ~word ^ xorValue;
}

unsigned long int CdBitUtil::foldWord(unsigned long int _word, unsigned long int colorFieldSize) {
    unsigned long int xorMask;
    unsigned long int maskSize = colorFieldSize;
    unsigned long int word = _word;
    while (maskSize > 1) {
        xorMask = getXor(colorFieldSize);
        word = word & (word << maskSize / 2);
        word = word & xorMask;
        maskSize = maskSize / 2;
    }
    return word;
}

unsigned long int CdBitUtil::getXor(unsigned long int colorFieldSize) {
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

unsigned long int CdBitUtil::getMask(unsigned long int colorFieldSize) {
    return (unsigned long int)pow(2, colorFieldSize) - 1;
}

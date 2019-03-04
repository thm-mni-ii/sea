#include "sealib/ccolorcditerator.h"
#include <iostream>

using Sealib::CcolorCdIterator;

CcolorCdIterator::CcolorCdIterator(CcolorChoiceDictionary *_choicedictionary)
    : blockSize(_choicedictionary->getBlockSize()),
      colorFieldSize(_choicedictionary->getColorFieldSize()),
      choicedictionary(_choicedictionary) {}

void CcolorCdIterator::init() {
    activeBlock = 0;
    activeWord = 0;
    nextWordIndex = 0;
    pointer = 0;
    done = false;
    barrier = choicedictionary->getBarrier(color);
    nextWord();
}

void CcolorCdIterator::setColor(unsigned long int _color) { color = _color; }

bool CcolorCdIterator::more() {
    if (done) return false;

    if (activeWord == 0) {
        if (!nextWord())
            return false;
        else
            return true;
    }
    return true;
}

unsigned long int CcolorCdIterator::next() {
    unsigned long int index = (unsigned long int)__builtin_clzl(activeWord);
    unsigned long int innerIndex = index / colorFieldSize;
    activeWord = activeWord & ~(1UL << (WORDSIZE - index - SHIFT_OFFSET));

    return ((activeBlock + nextWordIndex - 1) * WORDSIZE) / colorFieldSize +
           innerIndex;
    // unsigned long int wordIndex = activeBlock * blockSize + nextWordIndex -
    // 1; return (wordIndex * WORDSIZE) / colorFieldSize + innerIndex;
}

bool CcolorCdIterator::nextWord() {
    if (done) return false;
    if (activeBlock < 1 || nextWordIndex == blockSize + 1) {
        nextWordIndex = 0;
        if (!nextBlock()) return false;
    }

    while (activeWord == 0) {
        activeWord =
            choicedictionary->getWordValue(activeBlock + nextWordIndex);
        activeWord =
            CdBitUtil::cdColorIndices(activeWord, color, colorFieldSize);
        nextWordIndex++;

        if (activeWord == 0 && nextWordIndex == blockSize + 1) {
            if (!nextBlock()) return false;
            nextWordIndex = 0;
        }
    }

    return true;
}

bool CcolorCdIterator::nextBlock() {
    if (done) return false;
    if (pointer <= barrier / blockSize) {
        activeBlock = choicedictionary->getBlock(pointer, color);
        pointer++;
        return true;
    } else {
        done = true;
        return false;
    }
    return true;
}

CcolorCdIterator::~CcolorCdIterator() {}

#include "sealib/ccolorchoicedictionary.h"
#include <cmath>
#include <exception>
#include <iostream>

using Sealib::CcolorChoiceDictionary;

struct emptyChoiceDictionary : public std::exception {
    const char *what() const throw() {
        return "Choice dictionary is empty. Operation \'choice()\' "
               "is not possible";
    }
};

struct colorOutOfRange : public std::exception {
    const char *what() const throw() {
        return "Color cannot be used with computed fieldsize. The  biggest "
               "supported color by the operations \'choice()\' and \'insert()\' is "
               "equal to the defined color count minus one.";
    }
};

CcolorChoiceDictionary::CcolorChoiceDictionary(unsigned long int size,
                                               unsigned long int _colorCount)
    : wordSize(ULONGBITS),
      colorCount(_colorCount),
      colorFieldSize(computeColorFieldSize()),
      wordCount(size * colorFieldSize / wordSize + 1),
      mask(computeMask()) {
    createDataStructure();
}

/*
 * FIX SIZE E MUSS SIZE D SEIN
 * DURCH BLOCK IN D ITERIEREN, MAX ITERATIONEN: ANZAHL FARBEN
 */

void CcolorChoiceDictionary::insert(unsigned long int index, unsigned long int color) {
    if (color >= colorCount) throw colorOutOfRange();
    unsigned long int wordIndex = index * colorFieldSize / wordSize;
    unsigned long int blockIndex = (wordIndex / colorCount);

    if (isWritten(blockIndex, color)) {
        if (isChained(blockIndex, color)) {
            if (color != 0) {
                unsigned long int freeWrittenBlock = extend(color);
                unsigned long int chainedBlock = chainedWith(blockIndex, color) / colorCount;
                breakChain(blockIndex, color);
                makeChain(freeWrittenBlock, chainedBlock, color);
            }
        }
    } else {
        if (isChained(blockIndex, color)) {
        } else {
            if (color != 0) {
                unsigned long int freeWrittenBlock = extend(color);
                if (blockIndex == freeWrittenBlock) {
                } else {
                    if (!isInitialized(blockIndex)) initializeBlock(blockIndex);
                    makeChain(freeWrittenBlock, blockIndex, color);
                }
            }
        }
    }

    unsigned long int innerIndex = (index * colorFieldSize) % wordSize;
    unsigned long int colorMask = color << (wordSize - innerIndex - colorFieldSize);
    unsigned long int dataWord = choiceDictionary[wordIndex];

    dataWord = dataWord & ~(mask << (wordSize - innerIndex - colorFieldSize));
    dataWord = dataWord | colorMask;

    choiceDictionary[wordIndex] = dataWord;
    updatePointer(blockIndex);
}

unsigned long int CcolorChoiceDictionary::get(unsigned long int index) {
    unsigned long int word;
    unsigned long int colorCode;
    unsigned long int color;
    unsigned long int wordIndex = index * colorFieldSize / wordSize;
    unsigned long int innerIndex = (index * colorFieldSize) % wordSize;

    word = choiceDictionary[wordIndex];
    colorCode = word & (mask << (wordSize - innerIndex - colorFieldSize));
    color = colorCode >> (wordSize - innerIndex - colorFieldSize);
    return color;
}

unsigned long int CcolorChoiceDictionary::choice(unsigned long int color) {
    if (pointer[color] < colorCount) throw emptyChoiceDictionary();
    if (color >= colorCount) throw colorOutOfRange();
    unsigned long int index;
    unsigned long int wordIndex;
    unsigned long int word;
    long int innerIndex;
    bool found = false;
    unsigned int counter = 0;
    unsigned long int blockIndex = (pointer[color] - colorCount) / colorCount;

    if (isChained(blockIndex, color)) {
        wordIndex = chainedWith(blockIndex, color) / colorCount;

        wordIndex = wordIndex * colorCount;
    } else {
        wordIndex = blockIndex * colorCount;
    }

    while (!found && counter < colorCount) {
        word = choiceDictionary[wordIndex];
        innerIndex = CdBitUtil::cdXor(word, color, colorFieldSize);

        if (innerIndex >= 0) {
            index = (wordIndex * wordSize) / colorFieldSize + (unsigned long int)innerIndex;
            found = true;
        }

        counter++;
        wordIndex++;
    }
    return index;
}

unsigned long int CcolorChoiceDictionary::getBlockSize() { return colorCount - 1; }

unsigned long int CcolorChoiceDictionary::getColorFieldSize() { return colorFieldSize; }

unsigned long int CcolorChoiceDictionary::getWordValue(unsigned long int index) {
    return choiceDictionary[index];
}

unsigned long int CcolorChoiceDictionary::getBlock(unsigned long int blockIndex,
                                                   unsigned long int color) {
    if (isChained(blockIndex, color))
        return chainedWith(blockIndex, color) - color;
    else
        return blockIndex * colorCount;
}

unsigned long int CcolorChoiceDictionary::getBarrier(unsigned long int color) {
    return pointer[color];
}

unsigned long int CcolorChoiceDictionary::extend(unsigned long int color) {
    unsigned long int freeBlock = pointer[color] / colorCount;

    if (isChained(freeBlock, color)) {
        freeBlock = chainedWith(freeBlock, color) / colorCount;
        breakChain(freeBlock, color);
    } else {
        if (!isInitialized(freeBlock)) {
            initializeBlock(freeBlock);
        }
    }
    movePointer(color, true);
    return freeBlock;
}

void CcolorChoiceDictionary::updatePointer(unsigned long int blockIndex) {
    unsigned long int wordValue, wordIndex, barrier, lastBlock, chainTarget;
    long int segmentWithColor;
    bool blockHasColor;

    for (unsigned long int color = 1; color < colorCount; color++) {
        wordIndex = blockIndex * colorCount;
        barrier = wordIndex + colorCount;
        blockHasColor = false;

        while (wordIndex < barrier) {
            wordValue = choiceDictionary[wordIndex];

            if (wordValue > 0)
                segmentWithColor = CdBitUtil::cdXor(wordValue, color, colorFieldSize);
            else
                segmentWithColor = -1;

            if (segmentWithColor >= 0) {
                blockHasColor = true;
                break;
            }
            wordIndex++;
        }
        if (!blockHasColor) {
            if (!isWritten(blockIndex, color) && !isChained(blockIndex, color)) continue;
            lastBlock = pointer[color] / colorCount;
            if (lastBlock > 0) lastBlock--;
            if (isWritten(blockIndex, color)) {
                if (!isChained(blockIndex, color)) {
                    if (isChained(lastBlock, color)) {
                        chainTarget = chainedWith(lastBlock, color);
                        breakChain(lastBlock, color);
                    } else {
                        chainTarget = lastBlock;
                    }
                    makeChain(blockIndex, chainTarget, color);
                    movePointer(color, false);
                }
            } else {
                if (!isChained(blockIndex, color)) continue;
                chainTarget = chainedWith(blockIndex, color) / colorCount;
                if (chainTarget != lastBlock) {
                    if (isChained(lastBlock, color)) {
                        unsigned long int lastBlockChain =
                            chainedWith(lastBlock, color) / colorCount;
                        makeChain(chainTarget, lastBlockChain, color);
                    } else {
                        makeChain(chainTarget, lastBlock, color);
                    }
                }
                movePointer(color, false);
            }
        }
    }
}

void CcolorChoiceDictionary::movePointer(unsigned long int color, bool forward) {
    if (forward) {
        pointer[color] += colorCount;

    } else {
        if (pointer[color] <= colorCount)
            pointer[color] = color;
        else
            pointer[color] -= colorCount;
    }
}

bool CcolorChoiceDictionary::isWritten(unsigned long int blockIndex, unsigned long int color) {
    unsigned long int targetPointerWord = blockIndex * colorCount + color;

    if (targetPointerWord < pointer[color]) {
        return true;
    }
    return false;
}

bool CcolorChoiceDictionary::isInitialized(unsigned long int blockIndex) {
    for (unsigned long int color = 0; color < colorCount; color++) {
        if (pointer[color] <= color) continue;
        if ((isWritten(blockIndex, color) && !isChained(blockIndex, color)) ||
            (!isWritten(blockIndex, color) && isChained(blockIndex, color))) {
            return true;
        }
    }
    return false;
}

void CcolorChoiceDictionary::initializeBlock(unsigned long int blockIndex) {
    for (unsigned long int i = 0; i < colorCount; i++) {
        choiceDictionary[blockIndex * colorCount + i] = 0;
    }
}

bool CcolorChoiceDictionary::isChained(unsigned long int blockIndex, unsigned long int color) {
    unsigned long int chainTarget = blockIndex * colorCount + color;

    if (chainTarget >= wordCount) return false;

    unsigned long int targetPointer = pointerStructure[chainTarget];

    if (targetPointer >= wordCount) return false;

    if (pointerStructure[targetPointer] == chainTarget) {
        return true;
    }
    return false;
}

unsigned long int CcolorChoiceDictionary::chainedWith(unsigned long int blockIndex,
                                                      unsigned long int color) {
    return pointerStructure[blockIndex * colorCount + color];
}

void CcolorChoiceDictionary::makeChain(unsigned long int blockWritten,
                                       unsigned long int blockUnwritten, unsigned long int color) {
    unsigned long int writtenTarget = blockWritten * colorCount + color;
    unsigned long int unwrittenTarget = blockUnwritten * colorCount + color;

    pointerStructure[writtenTarget] = unwrittenTarget;
    pointerStructure[unwrittenTarget] = writtenTarget;
}

void CcolorChoiceDictionary::breakChain(unsigned long int blockIndex, unsigned long int color) {
    unsigned long int chain = chainedWith(blockIndex, color);
    pointerStructure[chain] = 0;
}

unsigned long int CcolorChoiceDictionary::computeColorFieldSize() {
    unsigned long int _colorFieldSize = (unsigned long int)ceil(log2f(colorCount));

    while (wordSize % _colorFieldSize != 0) {
        _colorFieldSize++;
    }
    return _colorFieldSize;
}

unsigned long int CcolorChoiceDictionary::computeMask() {
    return (unsigned long int)pow(2, colorFieldSize) - 1;
}

void CcolorChoiceDictionary::createDataStructure() {
    choiceDictionary = new unsigned long int[wordCount];
    pointerStructure = new unsigned long int[wordCount];
    pointer = new unsigned long int[colorCount];

    for (unsigned long int i = 0; i < colorCount; i++) {
        pointer[i] = i;
    }
}

CcolorChoiceDictionary::~CcolorChoiceDictionary() {
    delete[] choiceDictionary;
    delete[] pointerStructure;
    delete[] pointer;
}

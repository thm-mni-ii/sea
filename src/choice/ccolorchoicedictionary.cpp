#include <sealib/dictionary/ccolorchoicedictionary.h>
#include <cmath>
#include <exception>
#include <iostream>

using Sealib::CcolorChoiceDictionary;

/* class emptyChoiceDictionaryException : public std::exception {
 public:
    const char* what() const throw() {
        return "Choice dictionary is empty. Operation \'choice()\' "
               "is not possible";
    }
};

class colorOutOfRangeException : public std::exception {
 public:
    const char* what() {
        return "Color cannot be used with computed fieldsize. The  biggest "
               "supported color by the operations \'choice()\' and "
               "\'insert()\' is "
               "equal to the defined color count minus one.";
    }
}; */

CcolorChoiceDictionary::CcolorChoiceDictionary(uint64_t size,
                                               uint64_t _colorCount)
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

void CcolorChoiceDictionary::insert(uint64_t index, uint64_t color) {
    // if (color >= colorCount) throw colorOutOfRangeException();
    uint64_t wordIndex = index * colorFieldSize / wordSize;
    uint64_t blockIndex = (wordIndex / colorCount);

    if (isWritten(blockIndex, color)) {
        if (isChained(blockIndex, color)) {
            if (color != 0) {
                uint64_t freeWrittenBlock = extend(color);
                uint64_t chainedBlock =
                    chainedWith(blockIndex, color) / colorCount;
                breakChain(blockIndex, color);
                makeChain(freeWrittenBlock, chainedBlock, color);
            }
        }
    } else {
        if (isChained(blockIndex, color)) {
        } else {
            if (color != 0) {
                uint64_t freeWrittenBlock = extend(color);
                if (blockIndex == freeWrittenBlock) {
                } else {
                    if (!isInitialized(blockIndex)) initializeBlock(blockIndex);
                    makeChain(freeWrittenBlock, blockIndex, color);
                }
            }
        }
    }

    uint64_t innerIndex = (index * colorFieldSize) % wordSize;
    uint64_t colorMask = color << (wordSize - innerIndex - colorFieldSize);
    uint64_t dataWord = choiceDictionary[wordIndex];

    dataWord = dataWord & ~(mask << (wordSize - innerIndex - colorFieldSize));
    dataWord = dataWord | colorMask;

    choiceDictionary[wordIndex] = dataWord;
    updatePointer(blockIndex);
}

uint64_t CcolorChoiceDictionary::get(uint64_t index) {
    uint64_t word;
    uint64_t colorCode;
    uint64_t color;
    uint64_t wordIndex = index * colorFieldSize / wordSize;
    uint64_t innerIndex = (index * colorFieldSize) % wordSize;

    word = choiceDictionary[wordIndex];
    colorCode = word & (mask << (wordSize - innerIndex - colorFieldSize));
    color = colorCode >> (wordSize - innerIndex - colorFieldSize);
    return color;
}

uint64_t CcolorChoiceDictionary::choice(uint64_t color) {
    /* if (pointer[color] < colorCount) throw emptyChoiceDictionaryException();
    if (color >= colorCount) throw colorOutOfRangeException(); */
    uint64_t index;
    uint64_t wordIndex;
    uint64_t word;
    int64_t innerIndex;
    bool found = false;
    uint32_t counter = 0;
    uint64_t blockIndex = (pointer[color] - colorCount) / colorCount;

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
            index = (wordIndex * wordSize) / colorFieldSize +
                    static_cast<uint64_t>(innerIndex);
            found = true;
        }

        counter++;
        wordIndex++;
    }
    return index;
}

uint64_t CcolorChoiceDictionary::getBlockSize() { return colorCount - 1; }

uint64_t CcolorChoiceDictionary::getColorFieldSize() { return colorFieldSize; }

uint64_t CcolorChoiceDictionary::getWordValue(uint64_t index) {
    return choiceDictionary[index];
}

uint64_t CcolorChoiceDictionary::getBlock(uint64_t blockIndex, uint64_t color) {
    if (isChained(blockIndex, color))
        return chainedWith(blockIndex, color) - color;
    else
        return blockIndex * colorCount;
}

uint64_t CcolorChoiceDictionary::getBarrier(uint64_t color) {
    return pointer[color];
}

uint64_t CcolorChoiceDictionary::extend(uint64_t color) {
    uint64_t freeBlock = pointer[color] / colorCount;

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

void CcolorChoiceDictionary::updatePointer(uint64_t blockIndex) {
    uint64_t wordValue, wordIndex, barrier, lastBlock, chainTarget;
    int64_t segmentWithColor;
    bool blockHasColor;

    for (uint64_t color = 1; color < colorCount; color++) {
        wordIndex = blockIndex * colorCount;
        barrier = wordIndex + colorCount;
        blockHasColor = false;

        while (wordIndex < barrier) {
            wordValue = choiceDictionary[wordIndex];

            if (wordValue > 0)
                segmentWithColor =
                    CdBitUtil::cdXor(wordValue, color, colorFieldSize);
            else
                segmentWithColor = -1;

            if (segmentWithColor >= 0) {
                blockHasColor = true;
                break;
            }
            wordIndex++;
        }
        if (!blockHasColor) {
            if (!isWritten(blockIndex, color) && !isChained(blockIndex, color))
                continue;
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
                        uint64_t lastBlockChain =
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

void CcolorChoiceDictionary::movePointer(uint64_t color, bool forward) {
    if (forward) {
        pointer[color] += colorCount;

    } else {
        if (pointer[color] <= colorCount)
            pointer[color] = color;
        else
            pointer[color] -= colorCount;
    }
}

bool CcolorChoiceDictionary::isWritten(uint64_t blockIndex, uint64_t color) {
    uint64_t targetPointerWord = blockIndex * colorCount + color;

    if (targetPointerWord < pointer[color]) {
        return true;
    }
    return false;
}

bool CcolorChoiceDictionary::isInitialized(uint64_t blockIndex) {
    for (uint64_t color = 0; color < colorCount; color++) {
        if (pointer[color] <= color) continue;
        if ((isWritten(blockIndex, color) && !isChained(blockIndex, color)) ||
            (!isWritten(blockIndex, color) && isChained(blockIndex, color))) {
            return true;
        }
    }
    return false;
}

void CcolorChoiceDictionary::initializeBlock(uint64_t blockIndex) {
    for (uint64_t i = 0; i < colorCount; i++) {
        choiceDictionary[blockIndex * colorCount + i] = 0;
    }
}

bool CcolorChoiceDictionary::isChained(uint64_t blockIndex, uint64_t color) {
    uint64_t chainTarget = blockIndex * colorCount + color;

    if (chainTarget >= wordCount) return false;

    uint64_t targetPointer = pointerStructure[chainTarget];

    if (targetPointer >= wordCount) return false;

    if (pointerStructure[targetPointer] == chainTarget) {
        return true;
    }
    return false;
}

uint64_t CcolorChoiceDictionary::chainedWith(uint64_t blockIndex,
                                             uint64_t color) {
    return pointerStructure[blockIndex * colorCount + color];
}

void CcolorChoiceDictionary::makeChain(uint64_t blockWritten,
                                       uint64_t blockUnwritten,
                                       uint64_t color) {
    uint64_t writtenTarget = blockWritten * colorCount + color;
    uint64_t unwrittenTarget = blockUnwritten * colorCount + color;

    pointerStructure[writtenTarget] = unwrittenTarget;
    pointerStructure[unwrittenTarget] = writtenTarget;
}

void CcolorChoiceDictionary::breakChain(uint64_t blockIndex, uint64_t color) {
    uint64_t chain = chainedWith(blockIndex, color);
    pointerStructure[chain] = 0;
}

uint64_t CcolorChoiceDictionary::computeColorFieldSize() {
    uint64_t _colorFieldSize = static_cast<uint64_t>(ceil(log2f(colorCount)));

    while (wordSize % _colorFieldSize != 0) {
        _colorFieldSize++;
    }
    return _colorFieldSize;
}

uint64_t CcolorChoiceDictionary::computeMask() {
    return static_cast<uint64_t>(pow(2, colorFieldSize) - 1);
}

void CcolorChoiceDictionary::createDataStructure() {
    choiceDictionary = new uint64_t[wordCount];
    pointerStructure = new uint64_t[wordCount];
    pointer = new uint64_t[colorCount];

    for (uint64_t i = 0; i < colorCount; i++) {
        pointer[i] = i;
    }
}

CcolorChoiceDictionary::~CcolorChoiceDictionary() {
    delete[] choiceDictionary;
    delete[] pointerStructure;
    delete[] pointer;
}

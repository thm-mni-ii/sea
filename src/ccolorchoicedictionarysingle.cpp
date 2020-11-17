#include "sealib/ccolorchoicedictionarysingle.h"
#include <cmath>
#include <exception>
#include <iostream>

using Sealib::CcolorChoiceDictionarySingle;

// struct emptyChoiceDictionary : public std::exception {
//     const char *what() const throw() {
//         return "Choice dictionary is empty. Operations \'choice()\'"
//                " and \'remove()\' are not possible";
//     }
// };

CcolorChoiceDictionarySingle::CcolorChoiceDictionarySingle(unsigned long int length,
                                                           unsigned long int _colorCount)
    : wordSize(ULONGBITS),
      colorCount(_colorCount),
      colorFieldSize(computeColorFieldSize()),
      mask(computeMask()),
      pointerBits(computePointerBits()),
      usableArea(computeUsableArea()),
      pointerMask(computePointerMask()),
      blockSize(computeBlockSize()),
      wordCount(computeWordCount(length)) {
    createDataStructure();
}

void CcolorChoiceDictionarySingle::insert(unsigned long int index, unsigned long int color) {
    std::cout << "\nINSERT:\n"
              << "Index: " << index << "\n"
              << "color: " << color << "\n";
    // unsigned long int wordIndex = index * colorFieldSize / wordSize;
    unsigned long int wordIndex = index * colorFieldSize / wordSize;
    unsigned long int blockIndex = wordIndex / blockSize;
    std::cout << "wordIndex :" << wordIndex << "\n";
    std::cout << "blockIndex :" << blockIndex << "\n";

    if (isWritten(blockIndex, color)) {
        std::cout << "Written: TRUE\n";
        if (isChained(blockIndex, color)) {
            std::cout << "Chained: TRUE\n";
            unsigned long int freeWrittenBlock = extend(color);
            if (freeWrittenBlock == blockIndex) {
                // muss noch was gemacht werden?
            } else {
                if (!isInitialized(blockIndex)) initializeBlock(blockIndex);
                makeChain(freeWrittenBlock, chainedWith(blockIndex, color), color);
            }
        } else {
            std::cout << "Chained: FALSE\n";
        }
    } else {
        std::cout << "Written: FALSE\n";
        if (isChained(blockIndex, color)) {
            std::cout << "Chained: TRUE\n";
        } else {
            std::cout << "Chained: FALSE\n";
            unsigned long int freeWrittenBlock = extend(color);
            if (blockIndex == freeWrittenBlock) {
                // muss noch was gemacht werden?
            } else {
                if (!isInitialized(blockIndex)) initializeBlock(blockIndex);
                makeChain(freeWrittenBlock, blockIndex, color);
            }
        }
    }

    unsigned long int innerIndex = (index * colorFieldSize) % wordSize;

    unsigned long int colorMask = color << (wordSize - innerIndex - colorFieldSize);

    unsigned long int dataWord = choiceDictionary[wordIndex];

    // REMOVE LATER
    unsigned long int prevWord = dataWord;

    // set colorMasked part to null before writing to support changing colors
    dataWord = dataWord & ~(mask << (wordSize - innerIndex - colorFieldSize));
    dataWord = dataWord | colorMask;

    choiceDictionary[wordIndex] = dataWord;

    // REMOVE LATER
    std::cout << "innerIndex: " << innerIndex << "\n";
    std::cout << "prevWord: " << prevWord << "\n";
    std::cout << "colorMask: " << colorMask << "\n";
    std::cout << "dataWord: " << dataWord << "\n";
    std::cout << "\n"
              << "\n";
    // REMOVE LATER
}

bool CcolorChoiceDictionarySingle::isWritten(unsigned long int blockIndex,
                                             unsigned long int color) {
    if (blockIndex < pointer[color]) {
        return true;
    }
    /*else if (targetPointerWord == pointer[color]) {
      initializeBlock(blockIndex);
      movePointer(color, true);
      choiceDictionary[wordIndex] = 0;
      return true;
      }*/
    return false;
}

void CcolorChoiceDictionarySingle::initializeBlock(unsigned long int blockIndex) {
    for (unsigned long int i = 0; i < blockSize; i++) {
        choiceDictionary[blockIndex * blockSize + i] = 0;
    }
}

bool CcolorChoiceDictionarySingle::isInitialized(unsigned long int blockIndex) {
    for (unsigned long int color = 0; color < colorCount; color++) {
        if (pointer[color] == 0) continue;
        if ((isWritten(blockIndex, color) && !isChained(blockIndex, color)) ||
            (!isWritten(blockIndex, color) && isChained(blockIndex, color))) {
            return true;
        }
    }
    return false;
}

void CcolorChoiceDictionarySingle::movePointer(unsigned long int color, bool forward) {
    if (forward)
        pointer[color]++;
    else
        pointer[color]--;
}

void CcolorChoiceDictionarySingle::makeChain(unsigned long int from,
                                             unsigned long int to,
                                             unsigned long int color) {
    //unsigned long int unwrittenTarget = blockUnwritten;

    std::cout << "Making Chain Between Block " << from << " and " << to << "\n";

    // pointerStructure[writtenTarget] = unwrittenTarget;
    // pointerStructure[unwrittenTarget] = writtenTarget;
    setPointer(from, to, color);
    setPointer(to, from, color);
}

void CcolorChoiceDictionarySingle::breakChain(unsigned long int blockIndex,
                                              unsigned long int color) {
    unsigned long int chain = chainedWith(blockIndex, color);
    std::cout << "Breaking chain between " << blockIndex << " and " << chain / colorCount << "\n";
    // pointerStructure[chain] = 0;
    setPointer(chain, 0, color);
}

bool CcolorChoiceDictionarySingle::isChained(unsigned long int blockIndex,
                                             unsigned long int color) {
    unsigned long int chainTarget = blockIndex;

    if (chainTarget >= wordCount / blockSize || blockIndex == chainTarget) return false;

    // unsigned long int targetPointer = pointerStructure[chainTarget];

    // if (isEncoded(blockIndex) {
    unsigned long int targetPointer = assemblePointer(blockIndex, color);
    // } else {
    // return false;
    // ]

    if (targetPointer >= wordCount / blockSize) return false;

    // if (pointerStructure[targetPointer] == chainTarget) {
    //     return true;
    // }

    if (assemblePointer(targetPointer, color) == blockIndex) {
        return true;
    }

    return false;
}

unsigned long int CcolorChoiceDictionarySingle::chainedWith(unsigned long int blockIndex,
                                                            unsigned long int color) {
    return assemblePointer(blockIndex, color);
    // return pointerStructure[blockIndex * colorCount + color];
}

unsigned long int CcolorChoiceDictionarySingle::assemblePointer(unsigned long int blockIndex,
                                                                unsigned long int color) {
    unsigned long int word;
    unsigned long int fragment;
    unsigned long int pointerStart = blockIndex * blockSize + blockSize / colorCount * color;
    unsigned long int pointerValue = 0;

    for (unsigned long int i = 0; i < blockSize / colorCount; i++) {
        word = choiceDictionary[pointerStart + i];
        fragment = word & (pointerMask << (wordSize - pointerBits));
        pointerValue = pointerValue << pointerBits;
        pointerValue += fragment;
    }
    std::cout << "pointerValue: " << pointerValue << "\n";
    return pointerValue;
}

void CcolorChoiceDictionarySingle::setPointer(unsigned long int from, unsigned long int to,
                                              unsigned long int color) {
    unsigned long int value;
    unsigned long int fragment;
    unsigned long int pointerStart = from * blockSize + blockSize / colorCount * color;

    for (unsigned long int i = 0; i < blockSize / colorCount; i++) {
        value = choiceDictionary[pointerStart + i];
        fragment = to & pointerMask;
        std::cout << fragment;
        value = value & ~(pointerMask << (wordSize - pointerBits));
        value = value | (fragment << (wordSize - pointerBits));
        to = to >> pointerBits;
        choiceDictionary[pointerStart + i] = value;
    }
}

unsigned long int CcolorChoiceDictionarySingle::extend(unsigned long int color) {
    unsigned long int freeBlock = pointer[color];
    std::cout << "freeBlock: " << pointer[color] << "\n";

    if (isChained(freeBlock, color)) {
        freeBlock = chainedWith(freeBlock, color);
        breakChain(freeBlock, color);
    } else {
        freeBlock = pointer[color];
        if (!isInitialized(freeBlock)) {
            initializeBlock(freeBlock);
        }
    }
    movePointer(color, true);
    return freeBlock;
}

unsigned long int CcolorChoiceDictionarySingle::get(unsigned long int index) {
    unsigned long int word;
    unsigned long int colorCode;
    unsigned long int color;
    unsigned long int wordIndex = index * colorFieldSize / wordSize;
    unsigned long int innerIndex = (index * colorFieldSize) % wordSize;

    word = choiceDictionary[wordIndex];
    colorCode = word & (mask << (wordSize - innerIndex - colorFieldSize));
    color = colorCode >> (wordSize - innerIndex - colorFieldSize);
    /*
    std::cout << "wordIndex: " << wordIndex << "\n";
    std::cout << "word: " << word << "\n";
    std::cout << "colorCode: " << colorCode << "\n";
    std::cout << "color: " << color << "\n";
    */
    return color;
}

unsigned long int CcolorChoiceDictionarySingle::choice(unsigned long int color) {
    // if (pointer[color] < colorCount) throw emptyChoiceDictionary();
    unsigned long int index = 500;
    unsigned long int wordIndex;
    unsigned long int word;
    // unsigned long int notWord;
    // unsigned long int xorValue;
    // unsigned long int xorWord;
    // unsigned long int compareField;
    long int innerIndex;
    // unsigned int field;
    bool found = false;
    unsigned int counter = 0;
    unsigned long int blockIndex = pointer[color - 1] / colorCount;

    if (isChained(blockIndex, color)) blockIndex = chainedWith(blockIndex, color) - color;

    while (!found && counter < colorCount) {
        wordIndex = blockIndex + counter;
        word = choiceDictionary[wordIndex];
        /*
        notWord = ~word;
        xorValue = color;

        for (unsigned long int i = 0; i < wordSize / colorFieldSize; i++) {
            xorValue = xorValue << colorFieldSize;
            xorValue += color;
        }

        xorWord = notWord ^ xorValue;
        */
        // innerIndex = CdBitUtil::cdXorLoop(word, colorFieldSize);
        innerIndex = CdBitUtil::cdXor(word, color, colorFieldSize);

        if (innerIndex >= 0) {
            index = (wordIndex * wordSize) / colorFieldSize + (unsigned long int)innerIndex;
            found = true;
        }

        /*
        std::cout << "pointer: " << pointer[color] << "\n";
            std::cout << "index: " << index << "\n";
            std::cout << "wordIndex: " << wordIndex << "\n";
        std::cout << "wordIndex: " << wordIndex << "\n";
        std::cout << "blockIndex: " << blockIndex << "\n";
        std::cout << "word: " << word << "\n";
        std::cout << "notWord: " << notWord << "\n";
        std::cout << "xorValue: " << xorValue << "\n";
        std::cout << "xorWord: " << xorWord << "\n";
        std::cout << "\n"
            << "\n";
        */
        /*field = 0;
        while (!found && field < wordSize / colorFieldSize) {
            compareField = xorWord & (mask << field * colorFieldSize);
            // std::cout << "compareField: " << compareField << "\n";
            if (compareField & (compareField >> colorFieldSize / 2)) {
                index = wordIndex * (wordSize / colorFieldSize);
                index += wordSize / colorFieldSize - field - SHIFT_OFFSET;
                found = true;
            }
            field++;
        }*/
        counter++;
    }

    return index;
}

unsigned long int CcolorChoiceDictionarySingle::getBlockSize() { return colorCount; }

unsigned long int CcolorChoiceDictionarySingle::getColorFieldSize() { return colorFieldSize; }

unsigned long int CcolorChoiceDictionarySingle::getBarrier(unsigned long int color) {
    return pointer[color];
}

unsigned long int CcolorChoiceDictionarySingle::getWordValue(unsigned long int index) {
    return choiceDictionary[index];
}

unsigned long int CcolorChoiceDictionarySingle::getBlock(unsigned long int blockIndex,
                                                         unsigned long int color) {
    if (isChained(blockIndex, color))
        return chainedWith(blockIndex, color) - color;
    else
        return blockIndex;
}

/*
void CcolorChoiceDictionarySingle::remove(unsigned long int index) {
    unsigned long int wordIndex;
    unsigned long int wordValue;
    unsigned long int innerIndex;
    unsigned long int targetBits;

    unsigned long int maskedIndex = index * colorFieldSize;
    wordIndex = maskedIndex / (unsigned long int)wordSize;
    innerIndex = maskedIndex % (unsigned long int)wordSize;

    wordValue = choiceDictionary[wordIndex];
    targetBits = mask >> (wordSize - innerIndex - SHIFT_OFFSET);

    choiceDictionary[wordIndex] = wordValue & ~targetBits;
}
*/

/* <------------- Helper Functions -------------> */

unsigned long int CcolorChoiceDictionarySingle::computeColorFieldSize() {
    unsigned long int _colorFieldSize = (unsigned long int)ceil(log2f(colorCount));

    while (wordSize % _colorFieldSize != 0) {
        _colorFieldSize++;
    }
    return _colorFieldSize;
}

unsigned long int CcolorChoiceDictionarySingle::computeMask() {
    // Get next biggest mask with only a 1 followed by 0s and substract 1 to get a mask full of 1
    return (unsigned long int)pow(2, colorFieldSize) - 1;
}

unsigned long int CcolorChoiceDictionarySingle::computePointerBits() {
    auto function = [](unsigned long int x) {
        if (x <= 1)
            return x;
        else
            return x - 1;
    };
    return (unsigned long int)__builtin_clzl(
        Sealib::CdEncode::basetrans(ULONG_MAX, mask + 1, mask, function));
}

unsigned long int CcolorChoiceDictionarySingle::computeUsableArea() {
    unsigned long int usable = wordSize - pointerBits;
    while (usable % colorFieldSize != 0) {
        usable--;
    }
    return usable;
}

unsigned long int CcolorChoiceDictionarySingle::computeBlockSize() {
    return (wordSize / pointerBits + 1) * colorCount;
}

unsigned long int CcolorChoiceDictionarySingle::computePointerMask() {
    return (unsigned long int)pow(2, pointerBits) - 1;
}

unsigned long int CcolorChoiceDictionarySingle::computeWordCount(unsigned long int length) {
    // return length * colorFieldSize / (wordSize - pointerBits) + 1;
    unsigned long int wordCountPointerless = length * colorFieldSize / wordSize + 1;
    if (wordCountPointerless / blockSize >= 1)
        return blockSize * (wordCountPointerless / blockSize + 1);
    return wordCountPointerless;
}

void CcolorChoiceDictionarySingle::createDataStructure() {
    // unsigned long int structureSize = wordCount;
    choiceDictionary = new unsigned long int[wordCount];
    pointer = new unsigned long int[colorCount];
    //pointer = {0};

    for (unsigned long int i = 0; i < colorCount; i++) {
        pointer[i] = 0;
    }
}

CcolorChoiceDictionarySingle::~CcolorChoiceDictionarySingle() {
    delete[] choiceDictionary;
    delete[] pointer;
}

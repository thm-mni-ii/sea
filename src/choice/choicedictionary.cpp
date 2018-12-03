#include "sealib/dictionary/choicedictionary.h"
#include <exception>
#include <iostream>

using Sealib::ChoiceDictionary;

class emptyChoiceDictionary : public std::exception {
    const char* what() const throw() {
        return "Choice dictionary is empty. Operations \'choice()\'"
               " and \'remove()\' are not possible";
    }
};

ChoiceDictionary::ChoiceDictionary(uint64_t size)
    :   wordSize(sizeof(uint64_t) * 8),
        wordCount(size / wordSize + 1),
        pointer(0),
        primary(wordCount),
        secondary((wordCount/(uint64_t)wordSize+1)*TUPEL_FACTOR),
        validator(wordCount/(uint64_t)wordSize+1)
    {}

void ChoiceDictionary::insert(uint64_t index) {
    uint64_t primaryWord;
    uint64_t targetBit;

    uint64_t primaryIndex = index / (uint64_t)wordSize;
    uint64_t primaryInnerIndex = index % (uint64_t)wordSize;

    if (isInitialized(primaryIndex))
        primaryWord = primary[primaryIndex];
    else
        primaryWord = 0;

    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryInnerIndex);
    primary[primaryIndex] = primaryWord | targetBit;

    updateSecondary(primaryIndex);
}

bool ChoiceDictionary::get(uint64_t index) {
    uint64_t primaryWord;
    uint64_t targetBit;
    uint64_t primaryInnerIndex;

    uint64_t primaryIndex = index / (uint64_t)wordSize;

    if (!isInitialized(primaryIndex))
        return 0;

    primaryInnerIndex = index % (uint64_t)wordSize;

    primaryWord = primary[primaryIndex];
    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryInnerIndex);

    return (primaryWord & targetBit) != 0;
}

uint64_t ChoiceDictionary::choice() {
    uint64_t colorIndex;
    uint64_t primaryWord;
    uint64_t primaryIndex;
    uint64_t secondaryWord;
    uint64_t primaryInnerIndex;

    if (pointer == 0) throw emptyChoiceDictionary();

    uint64_t secondaryIndex = validator[pointer - POINTER_OFFSET] - TUPEL_OFFSET;
    secondaryWord = secondary[secondaryIndex];

    primaryIndex = (secondaryIndex / TUPEL_FACTOR) * wordSize +
                   (uint64_t)__builtin_clzl(secondaryWord);

    primaryWord = primary[primaryIndex];

    colorIndex = (uint64_t)primaryIndex * (uint64_t)wordSize;
    primaryInnerIndex = (uint64_t)__builtin_clzl(primaryWord);

    colorIndex += primaryInnerIndex;
    return colorIndex;
}

void ChoiceDictionary::remove(uint64_t index) {
    uint64_t primaryWord;
    uint64_t newPrimaryWord;
    uint64_t targetBit;

    if (pointer == 0) throw emptyChoiceDictionary();

    uint64_t primaryIndex = index / (uint64_t)wordSize;
    uint64_t primaryInnerIndex = index % (uint64_t)wordSize;

    if (!isInitialized(primaryIndex)) return;

    primaryWord = primary[primaryIndex];
    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryInnerIndex);
    newPrimaryWord = primaryWord & ~targetBit;

    primary[primaryIndex] = newPrimaryWord;

    if (newPrimaryWord == 0) {
        removeFromSecondary(primaryIndex);
    }
}

uint64_t ChoiceDictionary::getPrimaryWord(uint64_t primaryIndex) {
    return primary[primaryIndex];
}

uint64_t ChoiceDictionary::getSecondaryWord(uint64_t secondaryIndex) {
    return secondary[secondaryIndex];
}

uint64_t ChoiceDictionary::getPointerTarget(uint64_t nextPointer) {
    return validator[nextPointer] - TUPEL_OFFSET;
}

bool ChoiceDictionary::pointerIsValid(uint64_t nextPointer) {
    if (nextPointer >= pointer) return false;

    uint64_t secondaryIndex = validator[nextPointer];
    if (secondaryIndex > (wordCount / wordSize + 1) * TUPEL_FACTOR) return false;

    if (nextPointer == secondary[secondaryIndex])
        return true;
    else
        return false;
}

uint32_t ChoiceDictionary::getWordSize() { return wordSize; }

uint64_t ChoiceDictionary::getSecondarySize() {
    return wordCount / (uint64_t)wordSize + 1;
}

void ChoiceDictionary::updateSecondary(uint64_t primaryIndex) {
    uint64_t targetBit;
    uint64_t secondaryWord;
    uint64_t secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    uint64_t linkTarget = secondary[secondaryIndex + TUPEL_OFFSET];

    if (linkTarget <= wordCount / (uint64_t)wordSize &&
        validator[linkTarget] == secondaryIndex + TUPEL_OFFSET && pointer > 0) {
        secondaryWord = secondary[secondaryIndex];
    } else {
        secondaryWord = 0;
        secondary[secondaryIndex + TUPEL_OFFSET] = makeLink(secondaryIndex);
    }

    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryIndex);
    secondary[secondaryIndex] = secondaryWord | targetBit;
}

void ChoiceDictionary::removeFromSecondary(uint64_t primaryIndex) {
    uint64_t targetBit;
    uint64_t secondaryWord;
    uint64_t newSecondaryWord;

    uint64_t secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;

    secondaryWord = secondary[secondaryIndex];
    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryIndex);
    newSecondaryWord = secondaryWord & ~targetBit;
    secondary[secondaryIndex] = newSecondaryWord;

    if (newSecondaryWord == 0) breakLink(secondaryIndex);
}

uint64_t ChoiceDictionary::makeLink(uint64_t secondaryIndex) {
    uint64_t validatorIndex;

    pointer++;

    validatorIndex = pointer - POINTER_OFFSET;
    validator[validatorIndex] = secondaryIndex + TUPEL_OFFSET;

    return validatorIndex;
}

void ChoiceDictionary::breakLink(uint64_t secondaryIndex) {
    uint64_t validatorIndex = secondary[secondaryIndex + TUPEL_OFFSET];
    secondary[secondaryIndex + TUPEL_OFFSET] = 0;
    validator[validatorIndex] = 0;
    shrinkValidator(validatorIndex);
}

void ChoiceDictionary::shrinkValidator(uint64_t validatorIndex) {
    if (validatorIndex < pointer - 1) {
        uint64_t secondaryTarget = validator[pointer - 1];
        validator[validatorIndex] = secondaryTarget;
        secondary[secondaryTarget] = validatorIndex;
    }
    pointer--;
}

bool ChoiceDictionary::isInitialized(uint64_t primaryIndex) {
    uint64_t secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    uint64_t secondaryWord = secondary[secondaryIndex];
    uint64_t targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryIndex);

    return (secondaryWord & targetBit) != 0 && hasColor(primaryIndex) && pointer > 0;
}

bool ChoiceDictionary::hasColor(uint64_t primaryIndex) {
    uint64_t secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    uint64_t link = secondary[secondaryIndex + TUPEL_OFFSET];

    if (link > wordCount / wordSize) {
        return false;
    } else if (validator[link] != secondaryIndex + TUPEL_OFFSET) {
        return false;
    } else {
        return true;
    }
}


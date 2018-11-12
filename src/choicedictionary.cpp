#include "sealib/choicedictionary.h"
#include <exception>
#include <iostream>

using Sealib::ChoiceDictionary;

struct emptyChoiceDictionary : public std::exception {
    const char* what() const throw() {
        return "Choice dictionary is empty. Operations \'choice()\'"
               " and \'remove()\' are not possible";
    }
};

ChoiceDictionary::ChoiceDictionary(unsigned long int size) {
    pointer = 0;
    wordSize = sizeof(unsigned long int) * 8;
    createDataStructure(size);
}

void ChoiceDictionary::insert(unsigned long int index) {
    unsigned long int primaryWord;
    unsigned long int targetBit;

    unsigned long int primaryIndex = index / (unsigned long int)wordSize;
    unsigned long int primaryInnerIndex = index % (unsigned long int)wordSize;

    if (isInitialized(primaryIndex))
        primaryWord = primary[primaryIndex];
    else
        primaryWord = 0;

    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryInnerIndex);
    primary[primaryIndex] = primaryWord | targetBit;

    updateSecondary(primaryIndex);
}

bool ChoiceDictionary::get(unsigned long int index) {
    unsigned long int primaryWord;
    unsigned long int targetBit;
    unsigned long int primaryInnerIndex;

    unsigned long int primaryIndex = index / (unsigned long int)wordSize;

    if (!isInitialized(primaryIndex))
        return 0;

    primaryInnerIndex = index % (unsigned long int)wordSize;

    primaryWord = primary[primaryIndex];
    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryInnerIndex);

    return (primaryWord & targetBit) != 0;
}

unsigned long int ChoiceDictionary::choice() {
    unsigned long int colorIndex;
    unsigned long int primaryWord;
    unsigned long int primaryIndex;
    unsigned long int secondaryWord;
    unsigned long int primaryInnerIndex;

    if (pointer == 0) throw emptyChoiceDictionary();

    unsigned long int secondaryIndex = validator[pointer - POINTER_OFFSET] - TUPEL_OFFSET;
    secondaryWord = secondary[secondaryIndex];

    primaryIndex = (secondaryIndex / TUPEL_FACTOR) * wordSize +
                   (unsigned long int)__builtin_clzl(secondaryWord);

    primaryWord = primary[primaryIndex];

    colorIndex = (unsigned long int)primaryIndex * (unsigned long int)wordSize;
    primaryInnerIndex = (unsigned long int)__builtin_clzl(primaryWord);

    colorIndex += primaryInnerIndex;
    return colorIndex;
}

void ChoiceDictionary::remove(unsigned long int index) {
    unsigned long int primaryWord;
    unsigned long int newPrimaryWord;
    unsigned long int targetBit;

    if (pointer == 0) throw emptyChoiceDictionary();

    unsigned long int primaryIndex = index / (unsigned long int)wordSize;
    unsigned long int primaryInnerIndex = index % (unsigned long int)wordSize;

    if (!isInitialized(primaryIndex)) return;

    primaryWord = primary[primaryIndex];
    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryInnerIndex);
    newPrimaryWord = primaryWord & ~targetBit;

    primary[primaryIndex] = newPrimaryWord;

    if (newPrimaryWord == 0) {
        removeFromSecondary(primaryIndex);
    }
}

unsigned long int ChoiceDictionary::getPrimaryWord(unsigned long int primaryIndex) {
    return primary[primaryIndex];
}

unsigned long int ChoiceDictionary::getSecondaryWord(unsigned long int secondaryIndex) {
    return secondary[secondaryIndex];
}

unsigned long int ChoiceDictionary::getPointerTarget(unsigned long int nextPointer) {
    return validator[nextPointer] - TUPEL_OFFSET;
}

bool ChoiceDictionary::pointerIsValid(unsigned long int nextPointer) {
    if (nextPointer >= pointer) return false;

    unsigned long int secondaryIndex = validator[nextPointer];
    if (secondaryIndex > (wordCount / wordSize + 1) * TUPEL_FACTOR) return false;

    if (nextPointer == secondary[secondaryIndex])
        return true;
    else
        return false;
}

unsigned int ChoiceDictionary::getWordSize() { return wordSize; }

unsigned long int ChoiceDictionary::getSecondarySize() {
    return wordCount / (unsigned long int)wordSize + 1;
}

void ChoiceDictionary::createDataStructure(unsigned long int size) {
    unsigned long int secondarySize;

    wordCount = size / wordSize + 1;
    secondarySize = wordCount / (unsigned long int)wordSize + 1;

    createPrimary();
    createSecondary(secondarySize);
    createValidator(secondarySize);
}

void ChoiceDictionary::createPrimary() { primary = new unsigned long int[wordCount]; }

void ChoiceDictionary::createSecondary(unsigned long int secondarySize) {
    secondary = new unsigned long int[secondarySize * TUPEL_FACTOR];
}

void ChoiceDictionary::createValidator(unsigned long int validatorSize) {
    validator = new unsigned long int[validatorSize];
}

void ChoiceDictionary::updateSecondary(unsigned long int primaryIndex) {
    unsigned long int targetBit;
    unsigned long int secondaryWord;
    unsigned long int secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    unsigned long int linkTarget = secondary[secondaryIndex + TUPEL_OFFSET];

    if (linkTarget <= wordCount / (unsigned long int)wordSize &&
        validator[linkTarget] == secondaryIndex + TUPEL_OFFSET && pointer > 0) {
        secondaryWord = secondary[secondaryIndex];
    } else {
        secondaryWord = 0;
        secondary[secondaryIndex + TUPEL_OFFSET] = makeLink(secondaryIndex);
    }

    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryIndex);
    secondary[secondaryIndex] = secondaryWord | targetBit;
}

void ChoiceDictionary::removeFromSecondary(unsigned long int primaryIndex) {
    unsigned long int targetBit;
    unsigned long int secondaryWord;
    unsigned long int newSecondaryWord;

    unsigned long int secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;

    secondaryWord = secondary[secondaryIndex];
    targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryIndex);
    newSecondaryWord = secondaryWord & ~targetBit;
    secondary[secondaryIndex] = newSecondaryWord;

    if (newSecondaryWord == 0) breakLink(secondaryIndex);
}

unsigned long int ChoiceDictionary::makeLink(unsigned long int secondaryIndex) {
    unsigned long int validatorIndex;

    pointer++;

    validatorIndex = pointer - POINTER_OFFSET;
    validator[validatorIndex] = secondaryIndex + TUPEL_OFFSET;

    return validatorIndex;
}

void ChoiceDictionary::breakLink(unsigned long int secondaryIndex) {
    unsigned long int validatorIndex = secondary[secondaryIndex + TUPEL_OFFSET];
    secondary[secondaryIndex + TUPEL_OFFSET] = 0;
    validator[validatorIndex] = 0;
    shrinkValidator(validatorIndex);
}

void ChoiceDictionary::shrinkValidator(unsigned long int validatorIndex) {
    if (validatorIndex < pointer - 1) {
        unsigned long int secondaryTarget = validator[pointer - 1];
        validator[validatorIndex] = secondaryTarget;
        secondary[secondaryTarget] = validatorIndex;
    }
    pointer--;
}

bool ChoiceDictionary::isInitialized(unsigned long int primaryIndex) {
    unsigned long int secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    unsigned long int secondaryWord = secondary[secondaryIndex];
    unsigned long int targetBit = 1UL << (wordSize - SHIFT_OFFSET - primaryIndex);

    return (secondaryWord & targetBit) != 0 && hasColor(primaryIndex) && pointer > 0;
}

bool ChoiceDictionary::hasColor(unsigned long int primaryIndex) {
    unsigned long int secondaryIndex = (primaryIndex / wordSize) * TUPEL_FACTOR;
    unsigned long int link = secondary[secondaryIndex + TUPEL_OFFSET];

    if (link > wordCount / wordSize) {
        return false;
    } else if (validator[link] != secondaryIndex + TUPEL_OFFSET) {
        return false;
    } else {
        return true;
    }
}

ChoiceDictionary::~ChoiceDictionary() {
    delete[] primary;
    delete[] secondary;
    delete[] validator;
}

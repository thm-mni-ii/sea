#include "sealib/choicedictionary.h"
#include <exception>
#include <iostream>

using Sealib::ChoiceDictionary;

ChoiceDictionary::ChoiceDictionary(unsigned long int length) {
    pointer = 0;
    wordSize = sizeof(unsigned long int) * 8;
    createDataStructure(length);
}

void ChoiceDictionary::insert(unsigned long int index) {
    unsigned long int primaryValue;
    unsigned long int primaryBitPosition;

    unsigned long int wordIndex = index / (unsigned long int)wordSize;
    unsigned int innerId = index % wordSize;

    if (isInitialized(wordIndex))
        primaryValue = primary[wordIndex];
    else
        primaryValue = 0;

    primaryBitPosition = 1ULL << (wordSize - SHIFT_OFFSET - innerId);
    primary[wordIndex] = primaryValue | primaryBitPosition;

    updateSecondary(wordIndex);
}

bool ChoiceDictionary::get(unsigned long int index) {
    unsigned long int primaryValue;
    unsigned long int primaryBitPosition;

    unsigned long int wordIndex = index / wordSize;
    unsigned int innerId = index % wordSize;

    primaryValue = primary[wordIndex];
    primaryBitPosition = 1UL << (wordSize - SHIFT_OFFSET - innerId);

    return (primaryValue & primaryBitPosition) != 0;
}

unsigned long int ChoiceDictionary::choice() {
    unsigned long int colorIndex;
    unsigned long int primaryValue;
    unsigned long int primaryIndex;
    unsigned long int secondaryValue;
    int innerIndex;

    try {
        unsigned long int secondaryIndex = validator[pointer - POINTER_OFFSET] - TUPEL_OFFSET;
        secondaryValue = secondary[secondaryIndex * TUPEL_FACTOR];

        primaryIndex = (unsigned long int)__builtin_clzl(secondaryValue);
        primaryValue = primary[primaryIndex];
    } catch (const std::exception &e) {
        std::cerr << e.what();
    }

    colorIndex = primaryIndex * (unsigned long int)wordSize;
    innerIndex = __builtin_clzl(primaryValue);

    colorIndex += (unsigned long int)innerIndex;
    return colorIndex;
}

bool ChoiceDictionary::pointerIsValid(unsigned long int nextPointer) {
    if (nextPointer >= pointer) return false;

    unsigned long int secondaryWordIndex = validator[nextPointer];
    if (secondaryWordIndex > (wordCount / wordSize + 1) * TUPEL_FACTOR) return false;

    if (nextPointer == secondary[secondaryWordIndex])
        return true;
    else
        return false;
}

unsigned long int ChoiceDictionary::getPointerTarget(unsigned long int nextPointer) {
    return validator[nextPointer] - TUPEL_OFFSET;
}

unsigned long int ChoiceDictionary::getPrimaryWord(unsigned long int primaryIndex) {
    return primary[primaryIndex];
}

unsigned long int ChoiceDictionary::getSecondaryWord(unsigned long int secondaryIndex) {
    return secondary[secondaryIndex];
}

unsigned int ChoiceDictionary::getWordSize() { return wordSize; }

unsigned long int ChoiceDictionary::getSecondarySize() {
    return wordCount / (unsigned long int)wordSize + 1;
}

void ChoiceDictionary::createDataStructure(unsigned long int length) {
    unsigned long int secondaryLength;

    wordCount = length / wordSize + 1;
    secondaryLength = wordCount / (unsigned long int)wordSize + 1;

    createPrimary();
    createSecondary(secondaryLength);
    createValidator(secondaryLength);
}

void ChoiceDictionary::createPrimary() { primary = new unsigned long int[wordCount]; }

void ChoiceDictionary::createSecondary(unsigned long int secondaryLength) {
    secondary = new unsigned long int[secondaryLength * TUPEL_FACTOR];
}

void ChoiceDictionary::createValidator(unsigned long int validatorLength) {
    validator = new unsigned long int[validatorLength];
}

void ChoiceDictionary::updateSecondary(unsigned long int wordIndex) {
    unsigned long int secondaryBitPosition;
    unsigned long int secondaryValue;
    unsigned long int secondaryIndex = (wordIndex / wordSize) * TUPEL_FACTOR;

    if (pointer > secondaryIndex)
        secondaryValue = secondary[secondaryIndex];
    else
        secondaryValue = 0;

    secondaryBitPosition = 1ULL << (wordSize - SHIFT_OFFSET - wordIndex);

    secondary[secondaryIndex] = secondaryValue | secondaryBitPosition;
    secondary[secondaryIndex + TUPEL_OFFSET] = makeLink(secondaryIndex);
}

unsigned long int ChoiceDictionary::makeLink(unsigned long int secondaryIndex) {
    unsigned long int validatorIndex;

    if (pointer <= secondaryIndex || pointer == 0) pointer++;

    validatorIndex = pointer - POINTER_OFFSET;
    validator[validatorIndex] = secondaryIndex + TUPEL_OFFSET;

    return validatorIndex;
}

bool ChoiceDictionary::isInitialized(unsigned long int wordIndex) {
    unsigned long int secondaryIndex = (wordIndex / wordSize) * TUPEL_FACTOR;
    unsigned long int secondaryValue = secondary[secondaryIndex];
    unsigned long int secondaryBitPosition = 1ULL << (wordSize - SHIFT_OFFSET - wordIndex);

    return (secondaryValue & secondaryBitPosition) != 0 && hasColor(wordIndex) && pointer > 0;
}

bool ChoiceDictionary::hasColor(unsigned long int wordIndex) {
    unsigned long int secondaryWordIndex = (wordIndex / wordSize) * TUPEL_FACTOR;
    unsigned long int link = secondary[secondaryWordIndex + TUPEL_OFFSET];

    if (link > wordCount / wordSize) {
        return false;
    } else if (validator[link] != secondaryWordIndex + TUPEL_OFFSET) {
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

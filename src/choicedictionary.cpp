#include "sealib/choicedictionary.h"

using Sealib::ChoiceDictionary;

ChoiceDictionary::ChoiceDictionary(unsigned long int length) {
    pointer = 0;
    blockSize = sizeof(unsigned long int) * 8;
    createDataStructure(length);
}

void ChoiceDictionary::insert(unsigned long int index) {
    unsigned int blockIndex = (unsigned int)index / blockSize;
    unsigned int innerId = index % blockSize;
    unsigned long int value = 0;

    if (isInitialized(blockIndex)) value = primary[blockIndex];
    unsigned long int newColor = 1ULL << (blockSize - SHIFT_OFFSET - innerId);

    primary[blockIndex] = value | newColor;

    updateSecondary(blockIndex);
}

bool ChoiceDictionary::get(unsigned long int index) {
    unsigned long int blockIndex = index / blockSize;
    unsigned long int secondaryBlockIndex = blockIndex / blockSize;
    unsigned int linkedBlock = tertiary[blockIndex];
    if (linkedBlock >= wordCount || validator[linkedBlock] != (unsigned int)blockIndex)
        return false;
    unsigned long int secondaryBlock = 1UL << (blockSize - SHIFT_OFFSET - blockIndex);

    if ((secondary[secondaryBlockIndex] & secondaryBlock) == 0) return false;

    unsigned int innerId = index % blockSize;
    unsigned long int primaryValue = primary[blockIndex];
    unsigned long int primaryPosition = 1UL << (blockSize - SHIFT_OFFSET - innerId);

    if ((primaryValue & primaryPosition) == 0) return false;

    return true;
}

long int ChoiceDictionary::choice() {
    unsigned long int blockId = validator[pointer - 1];
    unsigned long int primaryValue = primary[blockId];

    long int colorIndex = (long int)blockId * (long int)blockSize;
    long int innerIndex = __builtin_clzl(primaryValue);
    colorIndex += innerIndex;

    return colorIndex;
}

unsigned long int ChoiceDictionary::getColoredBlock() {
    if (pointer - 1 > 0 && isInitialized(0))
        return validator[0];
    else
        return 0;
}

unsigned long int ChoiceDictionary::getColoredBlock(unsigned long int prevBlock) {
    unsigned int prevValidatorPos = tertiary[prevBlock];
    if (prevValidatorPos + 1 < pointer)
        return validator[prevValidatorPos + 1];
    else
        return 0;
}

unsigned long int ChoiceDictionary::getBlockValue(unsigned long int blockIndex) {
    if (isInitialized(blockIndex))
        return primary[blockIndex];
    else
        return 0;
}

unsigned int ChoiceDictionary::getBlockSize() { return blockSize; }

void ChoiceDictionary::createDataStructure(unsigned long int length) {
    wordCount = length / blockSize;
    if (length % blockSize != 0) wordCount += 1;

    unsigned int secondaryLength = (unsigned int)(wordCount / blockSize);
    if (wordCount % blockSize != 0) secondaryLength += 1;

    createPrimary();
    createSecondary(secondaryLength);
    createTertiary(secondaryLength);
}

void ChoiceDictionary::createPrimary() { primary = new unsigned long int[wordCount]; }

void ChoiceDictionary::createSecondary(unsigned long int secondaryLength) {
    secondary = new unsigned long int[secondaryLength];
}

void ChoiceDictionary::createTertiary(unsigned long int tertiaryLength) {
    unsigned long int size = tertiaryLength * blockSize;
    tertiary = new unsigned int[size];
    validator = new unsigned int[size];
}

void ChoiceDictionary::updateSecondary(unsigned long int blockIndex) {
    unsigned long int secondaryValue = 0;
    unsigned long int secondaryIndex = blockIndex / blockSize;
    if (pointer > 0) secondaryValue = secondary[secondaryIndex];

    unsigned long int updatedBlock = 1ULL << (blockSize - SHIFT_OFFSET - blockIndex);

    secondary[secondaryIndex] = secondaryValue | updatedBlock;

    updateTertiary(blockIndex);
}

void ChoiceDictionary::updateTertiary(unsigned long int blockIndex) {
    unsigned int targetBlock = makeLink(blockIndex);
    tertiary[blockIndex] = targetBlock;
}

unsigned int ChoiceDictionary::makeLink(unsigned long int target) {
    if (pointer <= target || pointer == 0) pointer++;
    validator[pointer - 1] = (unsigned int)target;
    return pointer - 1;
}

bool ChoiceDictionary::isInitialized(unsigned long int blockIndex) {
    return hasColor(blockIndex) && pointer > 0;
}

bool ChoiceDictionary::hasColor(unsigned long int blockIndex) {
    unsigned long int linkedBlock = tertiary[blockIndex];

    if (linkedBlock >= wordCount * blockSize) {
        return false;
    } else if (validator[linkedBlock] != blockIndex) {
        return false;
    } else {
        return true;
    }
}

ChoiceDictionary::~ChoiceDictionary() {
    delete[] primary;
    delete[] secondary;
    delete[] tertiary;
    delete[] validator;
}

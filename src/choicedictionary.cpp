#include <stdlib.h>
#include <vector>
#include <iostream>
#include "sealib/choicedictionary.h"

using Sealib::ChoiceDictionary;

ChoiceDictionary::ChoiceDictionary(unsigned long int length)
{
    pointer = 0;
    createDataStructure(length);
}

void ChoiceDictionary::insert(unsigned long int index)
{
    unsigned int blockIndex = (unsigned int)index / BLOCK_SIZE;
    unsigned int innerId = index % BLOCK_SIZE;

    unsigned long int value = primary[blockIndex];
    unsigned long int newColor = 1ULL << (BLOCK_SIZE - innerId);

    primary[blockIndex] = value | newColor;

    updateSecondary(blockIndex);

    // unsigned int linkedIndex;
    // linkedIndex = makeLink(index);
    // A[index] = linkedIndex;
    // int test = __builtin_clzl(10);
}

bool ChoiceDictionary::get(unsigned long int index)
{
    unsigned long int blockIndex = index / BLOCK_SIZE;
    unsigned int linkedBlock = tertiary[blockIndex];

    if (linkedBlock >= wordCount || validator[linkedBlock != blockIndex])
        return false;

    unsigned long int secondaryBlock = 1ULL << (BLOCK_SIZE - blockIndex);

    if ((secondary[blockIndex] & secondaryBlock) == 0)
        return false;

    unsigned int innerId = index % BLOCK_SIZE;
    unsigned long int primaryValue = primary[blockIndex];
    unsigned long int primaryPosition = 1ULL << (BLOCK_SIZE - innerId);

    if ((primaryValue & primaryPosition) == 0)
        return false;

    // unsigned int linkedIndex = A[index];

    // if (linkedIndex >= length || B[linkedIndex] != index) return false;
    return true;
}

long int ChoiceDictionary::choice()
{
    // bool found = false;
    // unsigned long int innerIndex = 1UL;
    // unsigned int shiftCount = 0;
    // std::cout << "hier";
    unsigned long int blockId = validator[pointer - 1];
    unsigned long int primaryValue = primary[blockId];

    long int colorIndex = (long int)blockId * (long int)BLOCK_SIZE;
    long int innerIndex = (long int)BLOCK_SIZE - 1 - __builtin_clzl(primaryValue);
    colorIndex += innerIndex;

    // std::cout << "hier";


    // logarithmus hier

    // unsigned long int blockId = __builtin_clzl(primaryValue);
    // while(!found) {
    //     if ((primaryValue & innerIndex) != 0){
    //         found = true;
    //     }
    //     else {
    //         innerIndex = innerIndex << 1;
    //         shiftCount++;
    //     }
    // }

    // return blockId * BLOCK_SIZE + (BLOCK_SIZE - shiftCount);
    return colorIndex;
    // return B[pointer - 1];
}

std::vector<unsigned int> ChoiceDictionary::iterate()
{
    std::vector<unsigned int> indices;
    for (unsigned int blockIndex = 0; blockIndex < wordCount; blockIndex++)
    {
        if (hasColor(blockIndex))
        {
            std::vector<unsigned int> blockIndices = getBlockColors(blockIndex);
            indices.insert(indices.end(), blockIndices.begin(), blockIndices.end());
        }
    }
    return indices;
}

void ChoiceDictionary::createDataStructure(unsigned long int length)
{
    wordCount = length / BLOCK_SIZE;
    if (length % BLOCK_SIZE != 0)
        wordCount += 1;

    unsigned int secondaryLength = (unsigned int)(wordCount / BLOCK_SIZE);
    if (wordCount % BLOCK_SIZE != 0)
        secondaryLength += 1;

    createPrimary();
    createSecondary(secondaryLength);
    createTertiary(secondaryLength);
}

void ChoiceDictionary::createPrimary()
{
    primary = new unsigned long int[wordCount];
}

void ChoiceDictionary::createSecondary(unsigned int secondaryLength)
{
    secondary = new unsigned long int[secondaryLength];
}

void ChoiceDictionary::createTertiary(unsigned int tertiaryLength)
{
    tertiary = new unsigned int[tertiaryLength * BLOCK_SIZE];
    validator = new unsigned int[tertiaryLength * BLOCK_SIZE];
}

void ChoiceDictionary::updateSecondary(unsigned int blockIndex)
{
    unsigned int secondaryIndex = blockIndex / BLOCK_SIZE;
    unsigned long int updatedBlock = 1ULL << (BLOCK_SIZE - blockIndex);
    secondary[secondaryIndex] = secondary[secondaryIndex] | updatedBlock;

    updateTertiary(blockIndex);
}

void ChoiceDictionary::updateTertiary(unsigned int blockId)
{
    tertiary[blockId] = makeLink(blockId);
}

unsigned int ChoiceDictionary::makeLink(unsigned int target)
{
    validator[pointer] = target;
    if (pointer < target || pointer == 0)
        pointer++;
    return pointer;
}

bool ChoiceDictionary::hasColor(unsigned int blockIndex)
{
    if (validator[tertiary[blockIndex]] == blockIndex)
        return true;
    else
        return false;
}

std::vector<unsigned int> ChoiceDictionary::getBlockColors(unsigned int blockIndex)
{
    std::vector<unsigned int> blockIndices;
    unsigned long int blockValue = primary[blockIndex];
    int targetBit;
    unsigned long int operatorBit;

    while (blockValue > 0)
    {
        targetBit = __builtin_clzl(blockValue);
        operatorBit = 1UL << targetBit;
        blockValue = blockValue & ~operatorBit;
        blockIndices.push_back((unsigned int)targetBit);
    }
    return blockIndices;
}

ChoiceDictionary::~ChoiceDictionary()
{
    delete[] primary;
    delete[] secondary;
    delete[] tertiary;
    delete[] validator;
}

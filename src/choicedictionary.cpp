#include <stdlib.h>
#include "sealib/choicedictionary.h"

using Sealib::ChoiceDictionary;

// 4096 farben max?!

ChoiceDictionary::ChoiceDictionary(unsigned long long int _length) : length(_length)
{
    referenceA = new unsigned int[BLOCK_SIZE];
    referenceB = new unsigned int[BLOCK_SIZE];
    pointer = 0;
    secondary = 0;
    createPrimary();
}

void ChoiceDictionary::insert(unsigned long long int index)
{
    unsigned int blockId = (unsigned int) index / BLOCK_SIZE;
    unsigned int innerId = index % BLOCK_SIZE;

    uint64_t value = primary[blockId];
    uint64_t newColor = 1ULL << (BLOCK_SIZE - innerId);

    primary[blockId] = value | newColor;

    updateSecondary(blockId);

    // unsigned int linkedIndex;
    // linkedIndex = makeLink(index);
    // A[index] = linkedIndex;
    // int test = __builtin_clzl(10);
}

bool ChoiceDictionary::get(unsigned long long int index)
{
    unsigned int referenceBlock = (unsigned int) index / BLOCK_SIZE;
    unsigned int linkedBlock = referenceA[referenceBlock];

    if (linkedBlock >= length || referenceB[linkedBlock != referenceBlock])
        return false;

    uint64_t secondaryBlock = 1ULL << (BLOCK_SIZE - referenceBlock);

    if ((secondary & secondaryBlock) == 0)
        return false;

    unsigned int innerId = index % BLOCK_SIZE;
    uint64_t primaryValue = primary[referenceBlock];
    uint64_t primaryPosition = 1ULL << (BLOCK_SIZE - innerId);

    if ((primaryValue & primaryPosition) == 0)
        return false;

    // unsigned int linkedIndex = A[index];

    // if (linkedIndex >= length || B[linkedIndex] != index) return false;
    return true;
}

unsigned long long int ChoiceDictionary::choice()
{
    bool found = false;
    uint64_t innerIndex = 1ULL;
    unsigned int shiftCount = 0;

    uint64_t blockId = referenceB[pointer-1];
    uint64_t primaryValue = primary[blockId];

    while(!found) {
        if ((primaryValue & innerIndex) != 0){
            found = true;
        }
        else {
            innerIndex = innerIndex << 1;
            shiftCount++;
        }
    }

    return blockId * BLOCK_SIZE + (BLOCK_SIZE - shiftCount);
    // return B[pointer - 1];
}

void ChoiceDictionary::createPrimary()
{
    unsigned long long int size = length / BLOCK_SIZE;
    primary = new uint64_t[size];
}

void ChoiceDictionary::updateSecondary(unsigned int blockId)
{
    uint64_t updatedBlock = 1ULL << (BLOCK_SIZE - blockId);
    secondary = secondary | updatedBlock;

    updateReferences(blockId);
}

void ChoiceDictionary::updateReferences(unsigned int blockId)
{
    referenceA[blockId] = makeLink(blockId);
}

unsigned int ChoiceDictionary::makeLink(unsigned int target)
{
    referenceB[pointer] = target;
    if (pointer < target)
        pointer++;
    return pointer;
}

ChoiceDictionary::~ChoiceDictionary()
{
    delete[] referenceA;
    delete[] referenceB;
    delete[] primary;
}

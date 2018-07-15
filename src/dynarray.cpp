#include <stdlib.h>
#include "sealib/dynarray.h"

DynArray::DynArray(unsigned int _size, unsigned int _blockSize, int _initValue) : size(_size), blockSize(_blockSize), initValue(_initValue)
{
    array = new int[size];
    startUnwrittenArea = 0;
}

int DynArray::read(unsigned int index)
{
    unsigned int targetBlock = index / blockSize;

    if (isWrittenBlock(targetBlock))
    {
        return readFromWrittenBlock(isChained(targetBlock), index);
    }
    else
    {
        return readFromUnwrittenBlock(targetBlock, index);
    }
}

void DynArray::write(unsigned int index, int value)
{
    unsigned int targetBlock = index / blockSize;
    bool chained = isChained(targetBlock);

    if (isWrittenBlock(targetBlock))
    {
        // writeToWrittenBlock(targetBlock, index, value);
        if (chained)
        {
            extendWrittenArea();
            if (targetBlock == startUnwrittenArea - POINTEROFFSET)
            {
                array[index] = value;
                breakChain(targetBlock);
            }
            else
            {
                moveChainedBlock(targetBlock);

                makeChain(startUnwrittenArea - POINTEROFFSET, getChainedBlock(targetBlock));
                initBlock(targetBlock);

                array[index] = value;
                breakChain(targetBlock);
            }
        }
        else
        {
            array[index] = value;
            breakChain(targetBlock);
        }
    }
    else
    {
        // writeToUnwrittenBlock(targetBlock, index);
        if (chained)
        {
            unsigned int chainedBlock = getChainedBlock(targetBlock);
            unsigned int innerIndex = getInnerIndex(index);
            if (innerIndex < blockSize)
            {
                array[chainedBlock * blockSize + innerIndex] = value;
            }
            else
            {
                array[index] = value;
            }
        }
        else
        {
            extendWrittenArea();
            if (targetBlock == startUnwrittenArea - POINTEROFFSET)
            {
                array[index] = value;
                breakChain(targetBlock);
            }
            else
            {
                unsigned int chainedBlock = startUnwrittenArea - POINTEROFFSET;
                unsigned int innerIndex = getInnerIndex(index);

                initBlock(targetBlock);
                makeChain(chainedBlock, targetBlock);

                if (innerIndex < blockSize)
                {
                    array[chainedBlock * blockSize + innerIndex] = value;
                }
                else
                {
                    array[index] = value;
                }
            }
        }
    }
}

void DynArray::init()
{
    for (unsigned int index; index < size; index++)
    {
        array[index] = initValue;
    }
}

bool DynArray::isChained(unsigned int targetBlock)
{
    unsigned int chainedBlock;
    unsigned int bufferBlock = array[targetBlock * blockSize];
    if (bufferBlock >= size)
        return false;
    chainedBlock = array[bufferBlock];

    // chainedBlock GROESSER GLEICH 0
    // chainedBlock KLEINER Array Size
    // Wert von array[chainedBlock] GLEICH index von targetBlock
    // targetBlock in written area ODER chainedBlock in written area -> nur einer in written Area
    // nicht: targetBlock in written area UND chainedBlock in written area -> beide in written Area
    return chainedBlock >= 0 && chainedBlock < size && array[chainedBlock] == targetBlock * blockSize && (isWrittenBlock(targetBlock) || isWrittenBlock(chainedBlock / blockSize)) && !(isWrittenBlock(targetBlock) && isWrittenBlock(chainedBlock / blockSize));
    // && (targetBlock < startUnwrittenArea || chainedBlock / blockSize < startUnwrittenArea)
    // && !(targetBlock < startUnwrittenArea && chainedBlock / blockSize < startUnwrittenArea);
}

unsigned int DynArray::getChainedBlock(unsigned int targetBlock)
{
    return array[targetBlock * blockSize] / blockSize;
}

void DynArray::makeChain(unsigned int writtenIndex, unsigned int unwrittenIndex)
{
    if (writtenIndex == unwrittenIndex)
        return;
    array[writtenIndex * blockSize] = unwrittenIndex * blockSize;
    array[unwrittenIndex * blockSize] = writtenIndex * blockSize;
}

void DynArray::breakChain(unsigned int index)
{
    if (isChained(index))
    {
        unsigned int targetBlock = getChainedBlock(index);
        array[targetBlock * blockSize] = targetBlock * blockSize;
    }
}

bool DynArray::isWrittenBlock(unsigned int targetBlock)
{
    return targetBlock < startUnwrittenArea;
}

void DynArray::initBlock(unsigned int targetBlock)
{
    for (unsigned int innerIndex = 0; innerIndex < blockSize; innerIndex++)
    {
        array[targetBlock * blockSize + innerIndex] = initValue;
    }
}

void DynArray::extendWrittenArea()
{
    startUnwrittenArea += 1;
}

unsigned int DynArray::getInnerIndex(unsigned int index)
{
    return index % blockSize + INDEXOFFSET;
}

void DynArray::moveChainedBlock(unsigned int targetBlock)
{
    unsigned int target, source;

    for (unsigned int innerIndex = 0; innerIndex < blockSize; innerIndex++)
    {
        target = (startUnwrittenArea - POINTEROFFSET) * blockSize + innerIndex;
        source = targetBlock * blockSize + innerIndex;
        array[target] = array[source];
    }
}

int DynArray::readFromWrittenBlock(bool chained, unsigned int index)
{
    return chained ? initValue : array[index];
}

int DynArray::readFromUnwrittenBlock(unsigned int targetBlock, unsigned int index)
{
    if (isChained(targetBlock))
    {
        unsigned int chainedBlock = getChainedBlock(targetBlock);
        unsigned int innerIndex = getInnerIndex(index);
        // innerIndex hat einen OFFSET von +1, da Index 0 eines Blocks fuer den Pointer bestimmt ist
        // Dritter Index wird daher in den Unwritten Block geschrieben
        return innerIndex < blockSize ? array[chainedBlock * blockSize + innerIndex] : array[index];
    }
    else
    {
        return initValue;
    }
}

DynArray::~DynArray()
{
    delete[] array;
}

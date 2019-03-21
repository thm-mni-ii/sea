#include <iostream>
#include "sealib/iterator/choicedictionaryiterator.h"

using Sealib::ChoiceDictionaryIterator;

ChoiceDictionaryIterator::ChoiceDictionaryIterator(ChoiceDictionary const &_choicedictionary)
    : choicedictionary(_choicedictionary) {}

void ChoiceDictionaryIterator::init() {
    pointer = 0;
    primaryWord = 0;
    secondaryWord = 0;

    if (hasNextSecondary()) {
        setNextSecondaryWord();
        setNextPrimaryWord();
    }
}

bool ChoiceDictionaryIterator::more() {
    if (primaryWord == 0) {
        if (secondaryWord == 0) {
            if (hasNextSecondary())
                setNextSecondaryWord();
            else
                return false;
        }
        setNextPrimaryWord();
    }
    return true;
}

uint64_t ChoiceDictionaryIterator::next() {
    uint64_t newPrimaryValue;
    uint64_t wordSize = choicedictionary.getWordSize();

    uint64_t nextIndex = (uint64_t)__builtin_clzl(primaryWord);

    uint64_t targetBit = wordSize - nextIndex - SHIFT_OFFSET;

    newPrimaryValue = 1UL << targetBit;
    primaryWord = primaryWord & ~newPrimaryValue;

    return primaryIndex * wordSize + nextIndex;
}

bool ChoiceDictionaryIterator::hasNextSecondary() {
    return choicedictionary.pointerIsValid(pointer);
}

void ChoiceDictionaryIterator::setNextSecondaryWord() {
    secondaryIndex = choicedictionary.getPointerTarget(pointer);
    secondaryWord = choicedictionary.getSecondaryWord(secondaryIndex);
    pointer++;
}

void ChoiceDictionaryIterator::setNextPrimaryWord() {
    uint64_t targetBit;
    uint64_t wordSize = choicedictionary.getWordSize();
    uint64_t primaryInnerIndex = (uint64_t)__builtin_clzl(secondaryWord);

    primaryIndex = (secondaryIndex / 2) * wordSize + primaryInnerIndex;

    targetBit = wordSize - primaryInnerIndex - SHIFT_OFFSET;
    targetBit = 1UL << targetBit;

    secondaryWord = secondaryWord & ~targetBit;
    primaryWord = choicedictionary.getPrimaryWord(primaryIndex);
}

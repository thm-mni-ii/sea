#include <iostream>
#include "sealib/choicedictionaryiterator.h"

using Sealib::ChoiceDictionaryIterator;

ChoiceDictionaryIterator::ChoiceDictionaryIterator(ChoiceDictionary *_choicedictionary) {
    choicedictionary = _choicedictionary;
}

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

unsigned long int ChoiceDictionaryIterator::next() {
    unsigned long int newPrimaryValue;
    unsigned long int wordSize = choicedictionary->getWordSize();

    unsigned long int nextIndex = (unsigned long int)__builtin_clzl(primaryWord);

    unsigned long int targetBit = wordSize - nextIndex - SHIFT_OFFSET;

    newPrimaryValue = 1UL << targetBit;
    primaryWord = primaryWord & ~newPrimaryValue;

    return primaryIndex * wordSize + nextIndex;
}

bool ChoiceDictionaryIterator::hasNextSecondary() {
    return choicedictionary->pointerIsValid(pointer);
}

void ChoiceDictionaryIterator::setNextSecondaryWord() {
    secondaryIndex = choicedictionary->getPointerTarget(pointer);
    secondaryWord = choicedictionary->getSecondaryWord(secondaryIndex);
    pointer++;
}

void ChoiceDictionaryIterator::setNextPrimaryWord() {
    unsigned long int targetBit;
    unsigned long int wordSize = choicedictionary->getWordSize();
    unsigned long int primaryInnerIndex = (unsigned long int)__builtin_clzl(secondaryWord);

    primaryIndex = (secondaryIndex / 2) * wordSize + primaryInnerIndex;

    targetBit = wordSize - primaryInnerIndex - SHIFT_OFFSET;
    targetBit = 1UL << targetBit;

    secondaryWord = secondaryWord & ~targetBit;
    primaryWord = choicedictionary->getPrimaryWord(primaryIndex);
}

ChoiceDictionaryIterator::~ChoiceDictionaryIterator() {}

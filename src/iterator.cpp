#include <iostream>
#include "sealib/iterator.h"

using Sealib::Iterator;

Iterator::Iterator(ChoiceDictionary *_choicedictionary) { choicedictionary = _choicedictionary; }

void Iterator::init() {
    pointer = 0;
    primaryWord = 0;
    secondaryWord = 0;

    if (hasNextSecondary()) {
        setNextSecondaryWord();
        setNextPrimaryWord();
    }
}

bool Iterator::more() {
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

unsigned long int Iterator::next() {
    unsigned long int newPrimaryValue;
    unsigned long int wordSize = choicedictionary->getWordSize();

    unsigned long int nextIndex = (unsigned long int)__builtin_clzl(primaryWord);

    unsigned long int targetBit = wordSize - nextIndex - SHIFT_OFFSET;

    newPrimaryValue = 1UL << targetBit;
    primaryWord = primaryWord & ~newPrimaryValue;

    return primaryIndex * wordSize + nextIndex;
}

bool Iterator::hasNextSecondary() { return choicedictionary->pointerIsValid(pointer); }

void Iterator::setNextSecondaryWord() {
    secondaryIndex = choicedictionary->getPointerTarget(pointer);
    secondaryWord = choicedictionary->getSecondaryWord(secondaryIndex);
    pointer++;
}

void Iterator::setNextPrimaryWord() {
    unsigned long int targetBit;
    unsigned long int wordSize = choicedictionary->getWordSize();
    unsigned long int primaryInnerIndex = (unsigned long int)__builtin_clzl(secondaryWord);

    primaryIndex = (secondaryIndex / 2) * wordSize + primaryInnerIndex;

    targetBit = wordSize - primaryInnerIndex - SHIFT_OFFSET;
    targetBit = 1UL << targetBit;

    secondaryWord = secondaryWord & ~targetBit;
    primaryWord = choicedictionary->getPrimaryWord(primaryIndex);
}

Iterator::~Iterator() {}

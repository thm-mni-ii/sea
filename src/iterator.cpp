#include "sealib/iterator.h"

using Sealib::Iterator;

Iterator::Iterator(ChoiceDictionary *_choicedictionary) { choicedictionary = _choicedictionary; }

void Iterator::init() {
    activeBlock = choicedictionary->getColoredBlock();
    blockValue = choicedictionary->getBlockValue(activeBlock);
}

bool Iterator::more() {
    if (blockValue == 0) {
        unsigned long int newBlock = choicedictionary->getColoredBlock(activeBlock);
        if (newBlock > activeBlock) {
            activeBlock = newBlock;
            blockValue = choicedictionary->getBlockValue(activeBlock);
        } else {
            return false;
        }
    }
    return true;
}

unsigned long int Iterator::next() {
    int nextColor = __builtin_clzl(blockValue);
        unsigned long int operatorBit =
            1UL << (choicedictionary->getBlockSize() - (unsigned long int)nextColor - SHIFT_OFFSET);
        blockValue = blockValue & ~operatorBit;

    return (activeBlock * choicedictionary->getBlockSize()) + (unsigned long int)nextColor;
}

// void Iterator::close() {}

Iterator::~Iterator() {}

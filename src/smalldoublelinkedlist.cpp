//
// Created by jmeintrup on 06.06.18.
//

#include <sealib/smalldoublelinkedlist.h>
#include <iostream>

Sealib::SmallDoubleLinkedList::SmallDoubleLinkedList(unsigned int size) :
        links(size*2,1),
        current(size == 0 ? (unsigned char) - 1 : static_cast<unsigned char>(0)) {
}

unsigned int Sealib::SmallDoubleLinkedList::get() {
    if(current != (unsigned char) - 1) {
        unsigned char retVal = current;
        remove(current);
        return retVal;
    } else {
        return (unsigned char) - 1;
    }
}



unsigned int Sealib::SmallDoubleLinkedList::remove(unsigned int idx) {
    if(current == (unsigned char) - 1) {  // empty list
        return current;
    }

    int actualIdx = idx * 2;

    int prevIdx = actualIdx - links[actualIdx] * 2;
    auto n = static_cast<int>(links.size());
    int mod = prevIdx % n;
    if(prevIdx < 0) {
        prevIdx = n + mod;
    } else {
        prevIdx = mod;
    }
    if(prevIdx == n) {
        prevIdx = 0;
    }
    if(prevIdx == actualIdx) {  // last element
        current = (unsigned char) - 1;
        return idx;
    }
    links[prevIdx+1] += links[actualIdx+1];

    int nextIdx = actualIdx + links[actualIdx + 1] * 2;
    nextIdx = nextIdx % n;
    links[nextIdx] += links[actualIdx];

    current = static_cast<unsigned char>(nextIdx / 2);
    return current;
}

bool Sealib::SmallDoubleLinkedList::isEmpty() {
    return current == (unsigned char) - 1;
}
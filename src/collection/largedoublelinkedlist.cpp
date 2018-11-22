#include <sealib/largedoublelinkedlist.h>
#include <iostream>

Sealib::LargeDoubleLinkedList::LargeDoubleLinkedList(uint32_t size) :
    links(size * 2, 1),
    current(size == 0 ? (uint32_t) -1 : 0) {
}

uint32_t Sealib::LargeDoubleLinkedList::get() {
    if (current != (uint32_t) -1) {
        uint32_t retVal = current;
        remove(current);
        return retVal;
    } else {
        return (uint32_t) -1;
    }
}

uint32_t Sealib::LargeDoubleLinkedList::remove(uint32_t idx) {
    if (current == (uint32_t) -1) {  // empty list
        return current;
    }

    int32_t actualIdx = idx * 2;

    int32_t prevIdx = actualIdx - links[actualIdx] * 2;
    int32_t n = links.size();
    int32_t mod = prevIdx % n;
    if (prevIdx < 0) {
        prevIdx = n + mod;
    } else {
        prevIdx = mod;
    }
    if (prevIdx == n) {
        prevIdx = 0;
    }
    if (prevIdx == actualIdx) {  // last element
        current = (uint32_t) -1;
        return idx;
    }
    links[prevIdx + 1] += links[actualIdx + 1];

    int32_t nextIdx = actualIdx + links[actualIdx + 1] * 2;
    nextIdx = nextIdx % n;
    links[nextIdx] += links[actualIdx];

    current = static_cast<uint32_t>(nextIdx / 2);
    return current;
}

bool Sealib::LargeDoubleLinkedList::isEmpty() {
    return current == (uint32_t) -1;
}

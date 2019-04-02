#include "./largedoublelinkedlist.h"
#include <iostream>

Sealib::LargeDoubleLinkedList::LargeDoubleLinkedList(uint64_t size) :
    links(size * 2, 1),
    current(size == 0 ? INVALID : 0) {
}

uint64_t Sealib::LargeDoubleLinkedList::get() {
    if (current != INVALID) {
        uint64_t retVal = current;
        remove(current);
        return retVal;
    } else {
        return INVALID;
    }
}

uint64_t Sealib::LargeDoubleLinkedList::remove(uint64_t idx) {
    if (current == INVALID) {  // empty list
        return current;
    }

    auto actualIdx = static_cast<int32_t>(idx * 2);

    int32_t prevIdx = actualIdx;

    prevIdx -= static_cast<int32_t>(links[static_cast<uint64_t >(actualIdx)] * 2);

    auto n = static_cast<int32_t>(links.size());
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
        current = INVALID;
        return idx;
    }
    links[static_cast<uint64_t >(prevIdx) + 1] += links[static_cast<uint64_t >(actualIdx) + 1];

    int32_t nextIdx = actualIdx;
    nextIdx += static_cast<int32_t>(links[static_cast<uint64_t >(actualIdx) + 1] * 2);
    nextIdx = nextIdx % n;
    links[static_cast<uint64_t >(nextIdx)] += links[static_cast<uint64_t >(actualIdx)];

    current = static_cast<uint64_t>(nextIdx / 2);
    return current;
}

bool Sealib::LargeDoubleLinkedList::isEmpty() {
    return current == INVALID;
}

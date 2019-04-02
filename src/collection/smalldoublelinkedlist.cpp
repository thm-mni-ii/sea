#include "./smalldoublelinkedlist.h"

Sealib::SmallDoubleLinkedList::SmallDoubleLinkedList(uint8_t size) :
    links(size * 2, 1),
    current(size == 0 ? (uint8_t) -1 : static_cast<uint8_t>(0)) {
}

uint64_t Sealib::SmallDoubleLinkedList::get() {
    if (current != (uint8_t) -1) {
        uint8_t retVal = current;
        remove(current);
        return retVal;
    } else {
        return (uint8_t) -1;
    }
}

uint64_t Sealib::SmallDoubleLinkedList::remove(uint64_t idx) {
    if (current == (uint8_t) -1) {  // empty
        return current;
    }

    auto actualIdx = static_cast<int32_t>(idx * 2);

    int32_t prevIdx = actualIdx - links[static_cast<uint64_t >(actualIdx)] * 2;
    auto n = static_cast<int>(links.size());
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
        current = (uint8_t) -1;
        return idx;
    }

    int32_t val = links[static_cast<uint64_t >(prevIdx) + 1]
        + links[static_cast<uint64_t >(actualIdx) + 1];
    links[static_cast<uint64_t >(prevIdx) + 1] = static_cast<uint8_t>(val);

    int32_t nextIdx = actualIdx
        + links[static_cast<uint64_t >(actualIdx) + 1] * 2;
    nextIdx = nextIdx % n;

    val = links[static_cast<uint64_t >(nextIdx)]
        + links[static_cast<uint64_t >(actualIdx)];
    links[static_cast<uint64_t >(nextIdx)] = static_cast<uint8_t>(val);

    current = static_cast<uint8_t>(nextIdx / 2);
    return current;
}

bool Sealib::SmallDoubleLinkedList::isEmpty() {
    return current == (uint8_t) -1;
}

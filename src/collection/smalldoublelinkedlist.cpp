#include <sealib/smalldoublelinkedlist.h>

Sealib::SmallDoubleLinkedList::SmallDoubleLinkedList(uint8_t size) :
    links(size * 2, 1),
    current(size == 0 ? (uint8_t) -1 : static_cast<uint8_t>(0)) {
}

uint32_t Sealib::SmallDoubleLinkedList::get() {
    if (current != (uint8_t) -1) {
        uint8_t retVal = current;
        remove(current);
        return retVal;
    } else {
        return (uint8_t) -1;
    }
}

uint32_t Sealib::SmallDoubleLinkedList::remove(uint32_t idx) {
    if (current == (uint8_t) -1) {  // empty
        return current;
    }

    int actualIdx = idx * 2;

    int prevIdx = actualIdx - links[actualIdx] * 2;
    auto n = static_cast<int>(links.size());
    int mod = prevIdx % n;
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

    int val = links[prevIdx + 1] + links[actualIdx + 1];
    links[prevIdx + 1] = static_cast<uint8_t>(val);

    int nextIdx = actualIdx + links[actualIdx + 1] * 2;
    nextIdx = nextIdx % n;

    val = links[nextIdx] + links[actualIdx];
    links[nextIdx] = static_cast<uint8_t>(val);

    current = static_cast<uint8_t>(nextIdx / 2);
    return current;
}

bool Sealib::SmallDoubleLinkedList::isEmpty() {
    return current == (uint8_t) -1;
}

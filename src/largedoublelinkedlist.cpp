#include <sealib/largedoublelinkedlist.h>
#include <iostream>

Sealib::LargeDoubleLinkedList::LargeDoubleLinkedList(unsigned int size) :
    links(size * 2, 1),
    current(size == 0 ? (unsigned int) -1 : 0) {
}

unsigned int Sealib::LargeDoubleLinkedList::get() {
    if (current != (unsigned int) -1) {
        unsigned int retVal = current;
        remove(current);
        return retVal;
    } else {
        return (unsigned int) -1;
    }
}

unsigned int Sealib::LargeDoubleLinkedList::remove(unsigned int idx) {
    if (current == (unsigned int) -1) {  // empty list
        return current;
    }

    long actualIdx = idx * 2;

    long prevIdx = actualIdx - links[actualIdx] * 2;
    long n = links.size();
    long mod = prevIdx % n;
    if (prevIdx < 0) {
        prevIdx = n + mod;
    } else {
        prevIdx = mod;
    }
    if (prevIdx == n) {
        prevIdx = 0;
    }
    if (prevIdx == actualIdx) {  // last element
        current = (unsigned int) -1;
        return idx;
    }
    links[prevIdx + 1] += links[actualIdx + 1];

    long nextIdx = actualIdx + links[actualIdx + 1] * 2;
    nextIdx = nextIdx % n;
    links[nextIdx] += links[actualIdx];

    current = static_cast<unsigned int>(nextIdx / 2);
    return current;
}

bool Sealib::LargeDoubleLinkedList::isEmpty() {
    return current == (unsigned int) -1;
}
void Sealib::LargeDoubleLinkedList::clear() {
    current = (unsigned int) - 1;
    links.resize(0);
    links.shrink_to_fit();
}

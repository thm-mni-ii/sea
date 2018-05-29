#include <stdlib.h>
#include "sealib/choicedictionary.h"

ChoiceDictionary::ChoiceDictionary(unsigned int _length) : length(_length) {
    A = new unsigned int[length];
    B = new unsigned int[length];
    pointer = 0;
}

void ChoiceDictionary::insert(unsigned int index) {
    unsigned int linkedIndex;
    linkedIndex = makeLink(index);
    A[index] = linkedIndex;
}

bool ChoiceDictionary::get(unsigned int index) {
    unsigned int linkedIndex = A[index];

    if (linkedIndex >= length || B[linkedIndex] != index) return false;
    return true;
}

unsigned int ChoiceDictionary::choice() {
    return B[pointer - 1];
}

unsigned int ChoiceDictionary::makeLink(unsigned int target) {
    unsigned int linkedIndex = pointer;
    B[linkedIndex] = target;
    pointer++;
    return linkedIndex;
}

ChoiceDictionary::~ChoiceDictionary() {
    delete[] A;
    delete[] B;
}

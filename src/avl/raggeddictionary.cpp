#include "sealib/dictionary/raggeddictionary.h"
#include <cmath>

namespace Sealib {

RaggedDictionary::RaggedDictionary(uint64_t n)
    : size(n), keys(size / WORD_SIZE + 1), t(keys) {}

uint64_t RaggedDictionary::get(uint64_t i) {
    return t[i / WORD_SIZE].search(i);
}

void RaggedDictionary::insert(uint64_t i, uint64_t v) {
    if (entryCount < keys) {
        t[i / WORD_SIZE].insert(i, v);
        entryCount++;
    } else {
        throw RaggedDictionaryFull();
    }
}

void RaggedDictionary::remove(uint64_t i) { t[i / WORD_SIZE].remove(i); }

}  // namespace Sealib
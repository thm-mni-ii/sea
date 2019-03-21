#include "sealib/dictionary/raggeddictionary.h"
#include <cmath>

namespace Sealib {

RaggedDictionary::RaggedDictionary(uint64_t n)
    : size(n),
      l(static_cast<uint64_t>(ceil(log2(size)))),
      keys(size / l + 1),
      present(size),
      t(keys) {}

uint64_t RaggedDictionary::get(uint64_t i) const {
    if (present.get(i)) {
        return t[i / l].search(i);
    } else {
        return INVALID;
    }
}

void RaggedDictionary::insert(uint64_t i, uint64_t v) {
    if (entryCount + 1 < keys) {
        t[i / l].insert(i, v);
        present.insert(i);
        entryCount++;
    } else {
        throw RaggedDictionaryFull();
    }
}

void RaggedDictionary::remove(uint64_t i) {
    if (present.get(i)) {
        t[i / l].remove(i);
        present.remove(i);
        entryCount--;
    }
}

bool RaggedDictionary::member(uint64_t i) const { return present.get(i); }

uint64_t RaggedDictionary::someId() const { return present.choice(); }

ChoiceDictionaryIterator RaggedDictionary::allIds() const {
    ChoiceDictionaryIterator c(present);
    c.init();
    return c;
}

}  // namespace Sealib

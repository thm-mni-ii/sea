#include "sealib/dictionary/raggeddictionary.h"
#include <cmath>

namespace Sealib {

template <class T>
RaggedDictionaryBase<T>::RaggedDictionaryBase(uint64_t n)
    : size(n),
      l(static_cast<uint64_t>(ceil(log2(size)))),
      keys(size / l + 1),
      present(size),
      t(keys) {}

template <class T>
T RaggedDictionaryBase<T>::get(uint64_t i) const {
    if (present.get(i)) {
        return t[i / l].search(i);
    } else {
        return AVLTreeBase<T>::invalidValue();
    }
}

template <class T>
void RaggedDictionaryBase<T>::insert(uint64_t i, T v) {
    if (entryCount + 1 < keys) {
        t[i / l].insert(i, v);
        present.insert(i);
        entryCount++;
    } else {
        throw RaggedDictionaryFull();
    }
}

template <class T>
void RaggedDictionaryBase<T>::remove(uint64_t i) {
    if (present.get(i)) {
        t[i / l].remove(i);
        present.remove(i);
        entryCount--;
    }
}

template <class T>
bool RaggedDictionaryBase<T>::member(uint64_t i) const {
    return present.get(i);
}

template <class T>
uint64_t RaggedDictionaryBase<T>::someId() const {
    return present.choice();
}

template <class T>
ChoiceDictionaryIterator RaggedDictionaryBase<T>::allIds() const {
    ChoiceDictionaryIterator c(present);
    c.init();
    return c;
}

template class RaggedDictionaryBase<uint64_t>;
template class RaggedDictionaryBase<std::pair<uint64_t, uint64_t>>;

}  // namespace Sealib

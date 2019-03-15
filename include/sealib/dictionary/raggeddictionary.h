#ifndef SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_
#define SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/avltree.h"

namespace Sealib {

class RaggedDictionary {
 public:
    RaggedDictionary(uint64_t universeSize);

    uint64_t get(uint64_t i);

    void insert(uint64_t i, uint64_t v);

    void remove(uint64_t i);

 private:
    const uint64_t WORD_SIZE = sizeof(uint64_t) * 8;
    uint64_t size, keys;
    uint64_t entryCount = 0;
    std::vector<AVLTree> t;
};

class RaggedDictionaryFull : public std::exception {
    const char* what() const noexcept {
        return "Ragged dictionary is full! (more than O(n/log(n)) "
               "values present)";
    }
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_

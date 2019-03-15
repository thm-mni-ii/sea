#ifndef SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_
#define SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/avltree.h"
#include "sealib/dictionary/choicedictionary.h"

namespace Sealib {
/**
 * A dictionary that holds up to n/log(n) key-value pairs and supports the
 * following operations. In O(1) time:
 * - membership test
 * - returning an arbitrary key present in the dictionary
 * In O(log log n) time:
 * - inspecting the value stored for a key
 * - inserting a new tuple
 * - deleting a present tuple
 * (Elmasry, Hagerup, Kammer; 2015)
 *
 * @author Simon Heuser
 */
class RaggedDictionary {
 public:
    RaggedDictionary(uint64_t universeSize);

    uint64_t get(uint64_t i) const;

    void insert(uint64_t i, uint64_t v);

    void remove(uint64_t i);

    uint64_t someId() const;

 private:
    uint64_t size, l, keys;
    uint64_t entryCount = 0;
    ChoiceDictionary present;
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

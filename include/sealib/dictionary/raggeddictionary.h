#ifndef SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_
#define SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/avltree.h"
#include "sealib/dictionary/choicedictionary.h"
#include "sealib/iterator/choicedictionaryiterator.h"

namespace Sealib {
/**
 * A dictionary that holds up to n/log2(n) key-value pairs and supports the
 * following operations. (Elmasry, Hagerup, Kammer; 2015)
 * In O(1) time:
 * - membership test
 * - returning an arbitrary key present in the dictionary
 * - iterating over all present keys
 * In O(log log n) time:
 * - inspecting the value stored for a key
 * - inserting a new tuple
 * - deleting a present tuple
 *
 * @author Simon Heuser
 */
class RaggedDictionary {
 public:
    /**
     * Create a new ragged dictionary with the given universe size n. (You will
     * not be able to store more than n/log2(n) elements, so choose n
     * accordingly.)
     * @param universeSize Universe size of the dictionary
     */
    explicit RaggedDictionary(uint64_t universeSize);

    /**
     * Get the value stored for the given key.
     * @param k Key to search for
     * @return The stored value, or INVALID if the key is not present in the
     * dictionary
     */
    uint64_t get(uint64_t k) const;

    /**
     * Insert a tuple into the dictionary.
     * @param k Key to insert (if the key is already present, it will be updated
     * with the given value)
     * @param v Value to store for the given key
     */
    void insert(uint64_t k, uint64_t v);

    /**
     * Remove a tuple from the dictionary.
     * @param k Key to remove the tuple for (if present)
     */
    void remove(uint64_t k);

    /**
     * Check if the given key is present in the dictionary.
     * @param k Key to check
     * @return true if present, false otherwise
     */
    bool member(uint64_t k) const;

    /**
     * Get an arbitrary key held in the dictionary.
     * @return Arbitrary key
     */
    uint64_t someId() const;

    /**
     * Get all the keys held in the dictionary.
     * @return Constant-time iterator over the keys
     */
    ChoiceDictionaryIterator allIds() const;

 private:
    uint64_t size, l, keys;
    uint64_t entryCount = 0;
    ChoiceDictionary present;
    std::vector<AVLTree> t;
};

class RaggedDictionaryFull : public std::exception {
    const char* what() const noexcept {
        return "Ragged dictionary is full! (more than n/log2(n) "
               "values present)";
    }
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_RAGGEDDICTIONARY_H_

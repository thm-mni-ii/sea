#ifndef SEALIB_DICTIONARY_CHOICEDICTIONARY_H_
#define SEALIB_DICTIONARY_CHOICEDICTIONARY_H_
#include <stdexcept>
#include <vector>
#include "sealib/_types.h"

#define SHIFT_OFFSET 1UL
#define POINTER_OFFSET 1UL
#define TUPEL_OFFSET 1UL
#define TUPEL_FACTOR 2UL

namespace Sealib {
/**
 * A choice dictionary is a bitset containing n elements that supports reading
 * and setting a bit in constant time and additionally a so-called choice
 * operation that returns the position of an arbitrary bit that is set to 1
 * in constant time.
 * @author Dennis Appel
 */
class ChoiceDictionary {
 public:
    /**
     * Creates choice dictionary with given size
     * @param length Length of the choice dictionary
     */
    explicit ChoiceDictionary(uint64_t size);

    /**
     * Sets a bit at specified index to 1.
     * @param index Index of bit that should be set to 1.
     */
    void insert(uint64_t index);

    /**
     * Returns the bit at specified index.
     * @param index Index to read.
     */
    bool get(uint64_t index) const;

    /**
     * Returns an arbitrary bit position that is set to 1.
     * @throws ChoiceDictionaryEmpty if empty
     */
    uint64_t choice() const;

    /**
     * Sets a bit at specified index to 0.
     * @param index Index of bit that should be set to 0.
     * @throws ChoiceDictionaryEmpty if empty
     */
    void remove(uint64_t index);

    uint64_t getPrimaryWord(uint64_t primaryIndex) const {
        return primary[primaryIndex];
    }

    uint64_t getSecondaryWord(uint64_t secondaryIndex) const {
        return secondary[secondaryIndex];
    }

    uint64_t getPointerTarget(uint64_t nextPointer) const {
        return validator[nextPointer] - TUPEL_OFFSET;
    }

    bool pointerIsValid(uint64_t nextPointer) const;

    uint64_t getWordSize() const { return wordSize; }

    uint64_t getSecondarySize() {
        return wordCount / wordSize + 1;
    }

 private:
    /**
     * 64 for uint64_t
     */
    uint64_t wordSize;
    /**
     * Stores the number of words in primary.
     */
    uint64_t wordCount;
    /**
     * Points to the next available word in validator and
     * pointer-1 to the last linked word in validator.
     */
    uint64_t pointer;
    /**
     * Array Structure where each word represents a subset of
     * the entire bitset.
     */
    std::vector<uint64_t> primary;
    /**
     * Tupel structure where the first word in a block
     * contains a bitset where each bit that is set to 1 points to a subset
     * in primary with atleast one bit set to 1.
     * The second word is used for pointer validation with the validator
     * array.
     */
    std::vector<uint64_t> secondary;
    /**
     * Array used to validate entries in secondary with
     * validator[i] pointing to a tupel in secondary that has atleast
     * one bit set to 1.
     */
    std::vector<uint64_t> validator;

    /**
     * Updates secondary to represent updates in primary.
     * @param primaryIndex Index of the updated Word in primary.
     */
    void updateSecondary(uint64_t primaryIndex);

    void removeFromSecondary(uint64_t primaryIndex);

    /**
     * Creates a link between a tupel in secondary and validator.
     *
     * @param secondaryIndex Index of the new secondary word.
     */
    uint64_t makeLink(uint64_t secondaryIndex);

    void breakLink(uint64_t secondaryIndex);

    void shrinkValidator(uint64_t startIndex);

    bool isInitialized(uint64_t primaryIndex) const;

    bool hasColor(uint64_t primaryIndex) const;
};

class ChoiceDictionaryEmpty : public std::exception {
    const char* what() const noexcept {
        return "Choice dictionary is empty. Operations \'choice()\'"
               " and \'remove()\' are not possible";
    }
};
}  // namespace Sealib
#endif  // SEALIB_DICTIONARY_CHOICEDICTIONARY_H_

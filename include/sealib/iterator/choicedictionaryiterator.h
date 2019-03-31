#ifndef SEALIB_ITERATOR_CHOICEDICTIONARYITERATOR_H_
#define SEALIB_ITERATOR_CHOICEDICTIONARYITERATOR_H_

#include "sealib/iterator/iterator.h"
#include "sealib/dictionary/choicedictionary.h"

#define SHIFT_OFFSET 1UL

namespace Sealib {
/**
 * Iteration class to return all bit positions of a choice dictionary that
 * are set to 1.
 *
 * @author Dennis Appel
 */
class ChoiceDictionaryIterator : Iterator<uint64_t> {
 private:
    uint64_t primaryWord,  ///< Value of the currently used word in primary
        secondaryWord,     ///< Value of the currently used word in secondary
        pointer,           ///< currently used pointer Position
        primaryIndex,      ///< Index of the currently used word in primary
        secondaryIndex;    ///< Index of the currently used word in secondary
    ChoiceDictionary const
        &choicedictionary;  ///< Pointer to an existing choice dictionary

    bool hasNextSecondary();

    void setNextSecondaryWord();

    void setNextPrimaryWord();

 public:
    /**
     * Creates an Iterator for a choice dictionary.
     * @param choicedictionary Pointer to an existing choice dictionary.
     */
    explicit ChoiceDictionaryIterator(ChoiceDictionary const &choicedictionary);

    /**
     * Initializes the iterator.
     */
    void init();

    /**
     * Returns true when there are more bits that are set to 1 in the choice
     * dictionary and updates the used words if neccessary.
     */
    bool more();

    /**
     * Returns the next index of a bit set to 1.
     */
    uint64_t next();
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_CHOICEDICTIONARYITERATOR_H_

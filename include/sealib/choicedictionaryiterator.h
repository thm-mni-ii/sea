#ifndef SEALIB_CHOICEDICTIONARYITERATOR_H_
#define SEALIB_CHOICEDICTIONARYITERATOR_H_

#include "sealib/iterator.h"
#include "sealib/choicedictionary.h"

#define SHIFT_OFFSET 1UL

namespace Sealib {
/**
 * Iteration class to return all bit positions of a choice dictionary that
 * are set to 1.
 *
 * @author Dennis Appel
 */
class ChoiceDictionaryIterator : Iterator<unsigned long int> {
 private:
    /**
     * @param primaryWord Value of the currently used word in primary.
     * @param secondaryWord Value of the currently used word in secondary.
     * @param primaryIndex Index of the currently used word in primary.
     * @param secondaryIndex Index of the currently used word in secondary.
     * @param pointer currently used pointer Position.
     * @param choicedictionary Pointer to an existing choice dictionary.
     */
    unsigned long int primaryWord, secondaryWord, pointer, primaryIndex, secondaryIndex;
    ChoiceDictionary *choicedictionary;

    bool hasNextSecondary();

    void setNextSecondaryWord();

    void setNextPrimaryWord();

 public:
    /**
     * Creates an Iterator for a choice dictionary.
     * @param _choicedictionary Pointer to an existing choice dictionary.
     */
    explicit ChoiceDictionaryIterator(ChoiceDictionary *_choicedictionary);

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
    unsigned long int next();

    ~ChoiceDictionaryIterator();
};
}  // namespace Sealib
#endif  // SEALIB_CHOICEDICTIONARYITERATOR_H_

#ifndef SEALIB_ITERATOR_H_
#define SEALIB_ITERATOR_H_

#include "sealib/choicedictionary.h"

namespace Sealib {
/**
 * Iteration class to return indices of colors within a choice dictionary.
 */
class Iterator {
#define SHIFT_OFFSET 1UL

   private:
    /**
     * @param activeBlock Index of the currently read block of the choice
     * dictionary.
     * @param blockValue Value of given block.
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
    explicit Iterator(ChoiceDictionary *_choicedictionary);

    /**
     * Initializes the iterator by getting the value of the first block with
     * a color of the choice dictionary.
     */
    void init();

    /**
     * Returns true when there are more indices with a color in the choice
     * dictionary.
     */
    bool more();

    /**
     * Returns the next index of a color and changes blockValue
     * accordingly.
     */
    unsigned long int next();

    /**
     *
     */
    // void close();

    ~Iterator();
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_H_

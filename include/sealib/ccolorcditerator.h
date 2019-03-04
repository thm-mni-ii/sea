#ifndef SEALIB_CCOLORCDITERATOR_H_
#define SEALIB_CCOLORCDITERATOR_H_

#include "sealib/ccolorchoicedictionary.h"
#include "sealib/iterator.h"

#define SHIFT_OFFSET 1UL

namespace Sealib {
/**
 * Iteration class to return all bit positions of a choice dictionary that
 * are set to 1.
 *
 * @author Dennis Appel
 */
class CcolorCdIterator : Iterator<unsigned long int> {
 private:
    /**
     * @param primaryWord Value of the currently used word in primary.
     * @param secondaryWord Value of the currently used word in secondary.
     * @param primaryIndex Index of the currently used word in primary.
     * @param secondaryIndex Index of the currently used word in secondary.
     * @param pointer currently used pointer Position.
     * @param choicedictionary Pointer to an existing choice dictionary.
     */
    const unsigned long int blockSize, colorFieldSize;
    CcolorChoiceDictionary* choicedictionary;
    unsigned long int pointer, barrier, activeBlock, activeWord, nextWordIndex, color;
    bool done;

    bool nextBlock();

    bool nextWord();

 public:
    /**
     * Creates an Iterator for a choice dictionary.
     * @param _choicedictionary Pointer to an existing choice dictionary.
     */
    explicit CcolorCdIterator(CcolorChoiceDictionary* _choicedictionary);

    /**
     * Initializes the iterator.
     */
    void init();
    void setColor(unsigned long int color);

    /**
     * Returns true when there are more bits that are set to 1 in the choice
     * dictionary and updates the used words if neccessary.
     */
    bool more();

    /**
     * Returns the next index of a bit set to 1.
     */
    unsigned long int next();

    ~CcolorCdIterator();
};
}  // namespace Sealib
#endif  // SEALIB_CCOLORCDITERATOR_H_

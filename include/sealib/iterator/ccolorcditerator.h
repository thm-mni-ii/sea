#ifndef SEALIB_CCOLORCDITERATOR_H_
#define SEALIB_CCOLORCDITERATOR_H_

#include <cstdint>
#include <sealib/dictionary/ccolorchoicedictionary.h>
#include <sealib/iterator/iterator.h>

#define SHIFT_OFFSET 1UL

namespace Sealib {
/**
 * Iteration class to return all bit positions of a choice dictionary that
 * are set to 1.
 *
 * @author Dennis Appel
 */
class CcolorCdIterator : Iterator<uint64_t> {
 private:
    /**
     * @param primaryWord Value of the currently used word in primary.
     * @param secondaryWord Value of the currently used word in secondary.
     * @param primaryIndex Index of the currently used word in primary.
     * @param secondaryIndex Index of the currently used word in secondary.
     * @param pointer currently used pointer Position.
     * @param choicedictionary Pointer to an existing choice dictionary.
     */
    const uint64_t blockSize, colorFieldSize;
    CcolorChoiceDictionary* choicedictionary;
    uint64_t pointer, barrier, activeBlock, activeWord, nextWordIndex,
        color;
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

    /**
     * Set the iterator to a color to iterate over.
     * @param Color to iterate over
     */
    void setColor(uint64_t color);

    /**
     * Returns true if there are more fields of a color in the choice
     * dictionary.
     */
    bool more();

    /**
     * Returns the arbitrary index of a field with a color.
     */
    uint64_t next();

    ~CcolorCdIterator();
};
}  // namespace Sealib
#endif  // SEALIB_CCOLORCDITERATOR_H_

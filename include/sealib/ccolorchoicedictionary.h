#ifndef SEALIB_CCOLORCHOICEDICTIONARY_H_
#define SEALIB_CCOLORCHOICEDICTIONARY_H_

#define SHIFT_OFFSET 1UL

#include "sealib/cdbitutil.h"

namespace Sealib {
/**
 * A choice dictionary is a bitset containing n elements that supports reading
 * and setting a field of bits to a color in constant time and additionally a
 * so-called _choice_ operation that returns the position of an arbitrary index
 * of a field that has a chosen color other than 0 in constant time.
 * @author Dennis Appel
 */

const unsigned long int ULONGBITS = sizeof(unsigned long int) * 8;

class CcolorChoiceDictionary {
 private:
    unsigned long int *choiceDictionary, *pointerStructure, *pointer;

    const unsigned long int wordSize, colorCount, colorFieldSize, wordCount,
        mask;

    unsigned long int extend(unsigned long int color);

    void updatePointer(unsigned long int blockIndex);

    void movePointer(unsigned long int color, bool forward);

    bool isWritten(unsigned long int index, unsigned long int color);

    bool isInitialized(unsigned long int blockIndex);

    void initializeBlock(unsigned long int blockIndex);

    bool isChained(unsigned long int index, unsigned long int color);

    void makeChain(unsigned long int blockWritten,
                   unsigned long int blockUnwritten, unsigned long int color);

    unsigned long int chainedWith(unsigned long int blockIndex,
                                  unsigned long int color);

    void breakChain(unsigned long int index, unsigned long int color);

    /* Initialization Functions */
    unsigned long int computeColorFieldSize();

    unsigned long int computeMask();

    void createDataStructure();

 public:
    /**
     * Creates choice dictionary with given size and colors.
     * @param length Length of the choice dictionary
     * @param colors Amount of colors including zero
     */
    explicit CcolorChoiceDictionary(unsigned long int size,
                                    unsigned long int colors);

    /**
     * Sets a field at specified index to a color.
     * @param index Index of the field that should be set to a color.
     * @param color Color the field should be set to.
     */
    void insert(unsigned long int index, unsigned long int color);

    /**
     * Returns the color of the field at the specified index.
     * @param index Index of field.
     */
    unsigned long int get(unsigned long int index);

    /**
     * Returns an arbitrary position of a field with a color.
     * @param color Color of the field.
     */
    unsigned long int choice(unsigned long int color);

    /**
     * Returns the block size used within the choice dictionary.
     * The block size is directly related to the amount of colors.
     */
    unsigned long int getBlockSize();

    /**
     * Returns the size of a field that can store a color.
     * The field size is usually a power of 2.
     */
    unsigned long int getColorFieldSize();

    /**
     * Returns the value of an entire word containing multiple fields.
     * @wordIndex index of the word which value is to be returned.
     */
    unsigned long int getWordValue(unsigned long int wordIndex);

    /**
     * Returns the block that corresponds to a pointer of a color.
     * @param pointerIndex Index within the pointer structure.
     * @param color Color which the index corresponds to.
     */
    unsigned long int getBlock(unsigned long int pointerIndex,
                               unsigned long int color);

    /**
     * Returns the index of the first word of the last block that contains
     * either a pointer to a block with a color or the block itself.
     * @param color Color for which the barrier is to be returned.
     */
    unsigned long int getBarrier(unsigned long int color);

    ~CcolorChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CCOLORCHOICEDICTIONARY_H_

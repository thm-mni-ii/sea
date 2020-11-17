#ifndef SEALIB_CCOLORCHOICEDICTIONARY_H_
#define SEALIB_CCOLORCHOICEDICTIONARY_H_

#define SHIFT_OFFSET 1UL

#include <cstdint>
#include <sealib/collection/cdbitutil.h>

namespace Sealib {
/**
 * A choice dictionary is a bitset containing n elements that supports reading
 * and setting a field of bits to a color in constant time and additionally a
 * so-called _choice_ operation that returns the position of an arbitrary index
 * of a field that has a chosen color other than 0 in constant time.
 * @author Dennis Appel
 */

const uint64_t ULONGBITS = sizeof(uint64_t) * 8;

class CcolorChoiceDictionary {
 private:
    uint64_t *choiceDictionary, *pointerStructure, *pointer;

    const uint64_t wordSize, colorCount, colorFieldSize, wordCount,
        mask;

    uint64_t extend(uint64_t color);

    void updatePointer(uint64_t blockIndex);

    void movePointer(uint64_t color, bool forward);

    bool isWritten(uint64_t index, uint64_t color);

    bool isInitialized(uint64_t blockIndex);

    void initializeBlock(uint64_t blockIndex);

    bool isChained(uint64_t index, uint64_t color);

    void makeChain(uint64_t blockWritten,
                   uint64_t blockUnwritten, uint64_t color);

    uint64_t chainedWith(uint64_t blockIndex,
                                  uint64_t color);

    void breakChain(uint64_t index, uint64_t color);

    /* Initialization Functions */
    uint64_t computeColorFieldSize();

    uint64_t computeMask();

    void createDataStructure();

 public:
    /**
     * Creates choice dictionary with given size and colors.
     * @param length Length of the choice dictionary
     * @param colors Amount of colors including zero
     */
    explicit CcolorChoiceDictionary(uint64_t size,
                                    uint64_t colors);

    /**
     * Sets a field at specified index to a color.
     * @param index Index of the field that should be set to a color.
     * @param color Color the field should be set to.
     */
    void insert(uint64_t index, uint64_t color);

    /**
     * Returns the color of the field at the specified index.
     * @param index Index of field.
     */
    uint64_t get(uint64_t index);

    /**
     * Returns an arbitrary position of a field with a color.
     * @param color Color of the field.
     */
    uint64_t choice(uint64_t color);

    /**
     * Returns the block size used within the choice dictionary.
     * The block size is directly related to the amount of colors.
     */
    uint64_t getBlockSize();

    /**
     * Returns the size of a field that can store a color.
     * The field size is usually a power of 2.
     */
    uint64_t getColorFieldSize();

    /**
     * Returns the value of an entire word containing multiple fields.
     * @wordIndex index of the word which value is to be returned.
     */
    uint64_t getWordValue(uint64_t wordIndex);

    /**
     * Returns the block that corresponds to a pointer of a color.
     * @param pointerIndex Index within the pointer structure.
     * @param color Color which the index corresponds to.
     */
    uint64_t getBlock(uint64_t pointerIndex,
                               uint64_t color);

    /**
     * Returns the index of the first word of the last block that contains
     * either a pointer to a block with a color or the block itself.
     * @param color Color for which the barrier is to be returned.
     */
    uint64_t getBarrier(uint64_t color);

    ~CcolorChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CCOLORCHOICEDICTIONARY_H_

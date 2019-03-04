#ifndef SEALIB_CCOLORCHOICEDICTIONARY_H_
#define SEALIB_CCOLORCHOICEDICTIONARY_H_

#define SHIFT_OFFSET 1UL

#include "sealib/cdbitutil.h"

namespace Sealib {
/**
 * A choice dictionary is a bitset containing n elements that supports reading
 * and setting a bit in constant time and additionally a so-called choice
 * operation that returns the position of an arbitrary bit that is set to 1
 * in constant time.
 * @author Dennis Appel
 */

const unsigned long int ULONGBITS = sizeof(unsigned long int) * 8;

class CcolorChoiceDictionary {
 private:
    unsigned long int *choiceDictionary, *pointerStructure, *pointer;
    const unsigned long int wordSize, colorCount, colorFieldSize, wordCount, mask;

    /* Helper Funcions */

    unsigned long int extend(unsigned long int color);

    void updatePointer(unsigned long int blockIndex);

    void movePointer(unsigned long int color, bool forward);

    bool isWritten(unsigned long int index, unsigned long int color);

    bool isInitialized(unsigned long int blockIndex);

    void initializeBlock(unsigned long int blockIndex);

    bool isChained(unsigned long int index, unsigned long int color);

    void makeChain(unsigned long int blockWritten, unsigned long int blockUnwritten,
                   unsigned long int color);

    unsigned long int chainedWith(unsigned long int blockIndex, unsigned long int color);

    void breakChain(unsigned long int index, unsigned long int color);

    /* Initialization Functions */
    unsigned long int computeColorFieldSize();

    unsigned long int computeMask();

    void createDataStructure();

 public:
    /**
     * Creates choice dictionary with given size
     * @param length Length of the choice dictionary
     */
    explicit CcolorChoiceDictionary(unsigned long int size, unsigned long int colorCount);

    /**
     * Sets a bit at specified index to 1.
     * @param index Index of bit that should be set to 1.
     */
    void insert(unsigned long int index, unsigned long int color);

    /**
     * Returns the bit at specified index.
     * @param index Index to read.
     */
    unsigned long int get(unsigned long int index);

    /**
     * Returns an arbitrary bit position that is set to 1.
     */
    unsigned long int choice(unsigned long int color);

    /**
     */
    unsigned long int getBlockSize();

    /**
     */
    unsigned long int getColorFieldSize();

    /**
     */
    unsigned long int getWordValue(unsigned long int wordIndex);

    /**
     */
    unsigned long int getBlock(unsigned long int pointerIndex, unsigned long int color);

    /**
     */
    unsigned long int getBarrier(unsigned long int color);

    ~CcolorChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CCOLORCHOICEDICTIONARY_H_

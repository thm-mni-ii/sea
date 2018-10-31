#ifndef SEALIB_CHOICEDICTIONARY_H_
#define SEALIB_CHOICEDICTIONARY_H_

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
 private:
    /**
     * @param primary Array Structure where each word represents a subset of
     * the entire bitset.
     *
     * @param secondary Tupel structure where the first word in a block
     * contains a bitset where each bit that is set to 1 points to a subset
     * in primary with atleast one bit set to 1.
     * The second word is used for pointer validation with the validator
     * array.
     *
     * @param validator Array used to validate entries in secondary with
     * validator[i] pointing to a tupel in secondary that has atleast
     * one bit set to 1.
     *
     * @param wordCount Stores the number of words in primary.
     *
     * @param pointer Points to the next available word in validator and
     * pointer-1 to the last linked word in validator.
     *
     * @param wordSize Either 32 or 64 depending on system architecture
     */
    unsigned long int *primary, *secondary, *validator, wordCount, pointer;
    unsigned int wordSize;

    void createDataStructure(unsigned long int size);

    void createPrimary();

    void createSecondary(unsigned long int secondarySize);

    void createValidator(unsigned long int validatorSize);

    /**
     * Updates secondary to represent updates in primary.
     * @param primaryIndex Index of the updated Word in primary.
     */
    void updateSecondary(unsigned long int primaryIndex);

    void removeFromSecondary(unsigned long int primaryIndex);

    /**
     * Creates a link between a tupel in secondary and validator.
     *
     * @param secondaryIndex Index of the new secondary word.
     */
    unsigned long int makeLink(unsigned long int secondaryIndex);

    void breakLink(unsigned long int secondaryIndex);

    void shrinkValidator(unsigned long int startIndex);

    bool isInitialized(unsigned long int primaryIndex);

    bool hasColor(unsigned long int primaryIndex);

 public:
    /**
     * Creates choice dictionary with given size
     * @param length Length of the choice dictionary
     */
    explicit ChoiceDictionary(unsigned long int size);

    /**
     * Sets a bit at specified index to 1.
     * @param index Index of bit that should be set to 1.
     */
    void insert(unsigned long int index);

    /**
     * Returns the bit at specified index.
     * @param index Index to read.
     */
    bool get(unsigned long int index);

    /**
     * Returns an arbitrary bit position that is set to 1.
     */
    unsigned long int choice();

    /**
     * Sets a bit at specified index to 0.
     * @param index Index of bit that should be set to 0.
     */
    void remove(unsigned long int index);

    unsigned long int getPrimaryWord(unsigned long int primaryIndex);

    unsigned long int getSecondaryWord(unsigned long int secondaryIndex);

    unsigned long int getPointerTarget(unsigned long int nextPointer);

    bool pointerIsValid(unsigned long int nextPointer);

    unsigned int getWordSize();

    unsigned long int getSecondarySize();

    ~ChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CHOICEDICTIONARY_H_

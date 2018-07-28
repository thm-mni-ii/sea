#ifndef SEALIB_CHOICEDICTIONARY_H_
#define SEALIB_CHOICEDICTIONARY_H_

namespace Sealib {
/**
 *
 */
class ChoiceDictionary {
#define SHIFT_OFFSET 1UL
#define POINTER_OFFSET 1UL
#define TUPEL_OFFSET 1UL
#define TUPEL_FACTOR 2UL
   private:
    /**
     * @param primary Array Structure with a word size of 64 or 32 bits
     * depending on system architecture, where each bit represents a color.
     *
     * @param secondary Array Structure where each word points to up to
     * 64 words in primary, showing whether that word contains a color.
     *
     * @param tertiary Array where each index of tertiary poitns to a
     * corresponding bit in secondary with a block that contains a color.
     *
     * @param validator Array used to validate tertiary.
     * tertiary[index] returns either an unitialized number or an index in
     * validator and validator[tertiary[index]] pointing to tertiary[index].
     * If that condition is met, a color has been written into the word at
     * index.
     *
     * @param wordCount Helper variable to determine the size of secondary
     * @param pointer Helper variable to track the progress through validator
     * @param blockSize Helper variable, either 32 or 64 depending on system
     * architecture
     */
    unsigned long int *primary, *secondary, *validator, wordCount, pointer;
    unsigned int wordSize;

    void createDataStructure(unsigned long int length);

    void createPrimary();

    void createSecondary(unsigned long int secondaryLength);

    void createValidator(unsigned long int validatorLength);

    /**
     * Creates a link between tertiary and validator by setting
     * validator[pointer-1] = target and returning pointer - 1, which is written
     * into tertiary[target] to create a pointer between both arrays.
     * @param target Index in tertiary where validator is being linked to.
     */
    unsigned long int makeLink(unsigned long int target);

    bool isInitialized(unsigned long int blockIndex);

    /**
     * Updates secondary to represent changes after inserting new colors
     * into primary.
     */
    void updateSecondary(unsigned long int blockIndex);

    /**
     * Updates tertiary to represent changes in secondary after it has been
     * updated
     */
    // void updateTertiary(unsigned long int updatedBlock);

    bool hasColor(unsigned long int blockIndex);

   public:
    /**
     * Creates choice dictionary with given size
     * @param length Length of the choice dictionary
     */
    explicit ChoiceDictionary(unsigned long int length);

    /**
     * Inserts a color into the specified index
     * @param index Index to insert into
     */
    void insert(unsigned long int index);

    /**
     * Returns true if the index is colored.
     * Checks whether tertiary[index/blockSize] points to a word in validator
     * that points back at the word in tertiary. If that condition is met
     * secondary is used to validate. If the validation succeeds, the value of
     * primary[index/blockSize] is used to determine whether index points to
     * a color.
     * @param index Index to read.
     */
    bool get(unsigned long int index);

    /**
     * Returns an index that has a color by using validator[pointer - 1] to
     * determine a word in primary with a color.
     * A logarithm is used to get the position of a bit in that word.
     */
    unsigned long int choice();

    /**
     * Returns the index of the first word in primary that has a color.
     */
    bool pointerIsValid(unsigned long int nextPointer);

    /**
     * Overload that returns the index of the next word in primary with a color.
     */
    unsigned long int getPointerTarget(unsigned long int nextPointer);

    /**
     * Returns the value of primary[blockIndex] to be used for iteration.
     */
    unsigned long int getPrimaryWord(unsigned long int primaryIndex);

    unsigned long int getSecondaryWord(unsigned long int secondaryIndex);

    /**
     * Returns the blockSize to be used for iteration operations outside
     * of the ChoiceDictionary class.
     */
    unsigned int getWordSize();

    unsigned long int getSecondarySize();

    ~ChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CHOICEDICTIONARY_H_

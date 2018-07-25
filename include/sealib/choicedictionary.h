#ifndef SEALIB_CHOICEDICTIONARY_H_
#define SEALIB_CHOICEDICTIONARY_H_

namespace Sealib {

class ChoiceDictionary {
#define SHIFT_OFFSET 1UL

   private:
    /**
     * @param primary Array Structure with a word size of 64bits, where each bit
     * represents a color
     * @param secondary Long Int(64bit) with each set bit pointing to a word in
     * primary containing a color
     * @param referanceA contains an array of indicies of refereneB.
     * @param referenceB array stucture containing indicies of refernceA and
     * stores the bit positions of secondary.
     */
    // unsigned int *A, *B;
    unsigned long int *primary, *secondary, wordCount;
    unsigned int *tertiary, *validator, pointer, blockSize;

    void createDataStructure(unsigned long int length);
    void createPrimary();
    void createSecondary(unsigned int secondaryLength);
    void createTertiary(unsigned int tertiaryLength);

    /**
     * Creates a link between A and B by setting and B[pointer-1]=a and
     * returning b with a=index and b=pointer-1
     * @param target Index in A where B is being linked to
     */
    unsigned int makeLink(unsigned int target);

    bool isInitialized(unsigned long int blockIndex);

    void updateSecondary(unsigned int blockId);

    void updateTertiary(unsigned int updatedBlock);

    bool hasColor(unsigned long int blockIndex);

   public:
    /**
     * Creates choice dictionary with given size
     * @param _size Size of choice dictionary
     */
    explicit ChoiceDictionary(unsigned long int length);

    /**
     * Inserts a color into the specified index
     * @param index Index to insert into
     */
    void insert(unsigned long int index);

    /**
     * Reads the color of A[index] and returns it
     * @param index Index to read from
     */
    bool get(unsigned long int index);

    /**
     * Returns an index i=B[pointer-1] where A[index]=true
     */
    long int choice();

    unsigned long int getColoredBlock();

    unsigned long int getColoredBlock(unsigned long int prevBlock);

    unsigned long int getBlockValue(unsigned long int blockIndex);

    unsigned int getBlockSize();

    ~ChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CHOICEDICTIONARY_H_

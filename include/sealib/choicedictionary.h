#ifndef SEALIB_CHOICEDICTIONARY_H_
#define SEALIB_CHOICEDICTIONARY_H_

namespace Sealib {

class ChoiceDictionary {
 private:
    #define BLOCK_SIZE 64

    /**
     * @param primary Array Structure with a word size of 64bits, where each bit represents a color
     * @param secondary Long Int(64bit) with each set bit pointing to a word in primary containing a color
     * @param referanceA contains an array of indicies of refereneB.
     * @param referenceB array stucture containing indicies of refernceA and stores the bit positions of secondary.
     */
    // unsigned int *A, *B;
    uint64_t *primary, secondary;
    unsigned long long int length;
    unsigned int *referenceA, *referenceB;
    unsigned int pointer;

    void createPrimary();

    /**
     * Creates a link between A and B by setting and B[pointer-1]=a and
     * returning b with a=index and b=pointer-1
     * @param target Index in A where B is being linked to
     */
    unsigned int makeLink(unsigned int target);

    void updateSecondary(unsigned int blockId);

    void updateReferences(unsigned int updatedBlock);

 public:
    /**
     * Creates choice dictionary with given size
     * @param _size Size of choice dictionary
     */
    explicit ChoiceDictionary(unsigned long long int _length);

    /**
     * Inserts a color into the specified index
     * @param index Index to insert into
     */
    void insert(unsigned long long int index);

    /**
     * Reads the color of A[index] and returns it
     * @param index Index to read from
     */
    bool get(unsigned long long int index);

    /**
     * Returns an index i=B[pointer-1] where A[index]=true
     */
    unsigned long long int choice();

    ~ChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CHOICEDICTIONARY_H_

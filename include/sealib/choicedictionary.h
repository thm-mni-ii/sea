#ifndef SEALIB_CHOICEDICTIONARY_H_
#define SEALIB_CHOICEDICTIONARY_H_

namespace Sealib {

class ChoiceDictionary {
 private:
    unsigned int *A, *B;
    unsigned int length, pointer;

    /**
     * Creates a link between A and B by setting and B[pointer-1]=a and
     * returning b with a=index and b=pointer-1
     * @param target Index in A where B is being linked to
     */
    unsigned int makeLink(unsigned int target);

 public:
    /**
     * Creates choice dictionary with given size
     * @param _size Size of choice dictionary
     */
    explicit ChoiceDictionary(unsigned int _length);

    /**
     * Inserts a color into the specified index
     * @param index Index to insert into
     */
    void insert(unsigned int index);

    /**
     * Reads the color of A[index] and returns it
     * @param index Index to read from
     */
    bool get(unsigned int index);

    /**
     * Returns an index i=B[pointer-1] where A[index]=true
     */
    unsigned int choice();

    ~ChoiceDictionary();
};
}  // namespace Sealib
#endif  // SEALIB_CHOICEDICTIONARY_H_

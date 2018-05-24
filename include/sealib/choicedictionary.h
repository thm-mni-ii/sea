#ifndef SEA_CHOICEDICTIONARY_H
#define SEA_CHOICEDICTIONARY_H

class ChoiceDictionary
{
  private:
    unsigned int *A, *B;
    unsigned int pointer;

    /**
     * Reorders the Array and changes the pointer when an entry
     * A[index] < pointer is changed to a value v=false
     * @param index Index of changed value in A
     * @param linkedIndex Index of B where A[index] is currently linked to
     */
    // void reorder(unsigned int index, unsigned int linedIndex);

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
    ChoiceDictionary(unsigned int _size);

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

#endif //SEA_CHOICEDICTIONARY_H

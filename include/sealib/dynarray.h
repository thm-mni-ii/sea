#ifndef SEALIB_DYNARRAY_H_
#define SEALIB_DYNARRAY_H_

class DynArray
{
  private:
    #define INDEXOFFSET 1
    #define POINTEROFFSET 1

    int *array;
    int initValue;
    unsigned int size, blockSize, startUnwrittenArea;

    bool isChained(unsigned int targetBlock);

    unsigned int getChainedBlock(unsigned int targetBlock);

    void makeChain(unsigned int writtenBlock, unsigned int unwrittenBlock);

    void breakChain(unsigned int targetBlock);

    bool isWrittenBlock(unsigned int targetBlock);

    void initBlock(unsigned int targetBlock);

    void extendWrittenArea();

    unsigned int getInnerIndex(unsigned int index);

    void moveChainedBlock(unsigned int targetBlock);

    int readFromWrittenBlock(bool chained, unsigned int index);

    int readFromUnwrittenBlock(unsigned int targetBlock, unsigned int index);

    // void writeToWrittenBlock(unsigned int targetBlock, unsigned int index, unsigned int value);

    // void writeToUnwrittenBlock(unsigned int targetBlock, unsigned int index, unsigned int value);

  public:
    explicit DynArray(unsigned int _size, unsigned int _blockSize, int _initValue);

    int read(unsigned int index);

    void write(unsigned int index, int value);

    void init();

    ~DynArray();
};

#endif // SEALIB_DYNARRAY_H_

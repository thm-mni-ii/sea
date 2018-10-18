#ifndef SEALIB_BITSET_H_
#define SEALIB_BITSET_H_

#include <vector>
#include <cassert>
#include <limits>

namespace Sealib {
/**
 * Simple Bitset to be used by all classes that need a dynamically allocated bitset.
 * Adds the ability to simply address whole blocks in the bitset for table lookup convenience.
 * Includes an implementation of operator[] for convenient use.
 * Partly based on dynamic_bitset from the boost library.
 * @author Johannes Meintrup
 */
template<typename BlockType = unsigned long>
class Bitset {
    typedef unsigned long sizeType;
    typedef bool bitType;

 private:
    static const unsigned int bitsPerBlock = sizeof(BlockType) * 8;
    static const BlockType BlockTypeOne = BlockType(1);

    sizeType bits;
    std::vector<BlockType> mbits;

    inline bitType get(const BlockType &i, sizeType b) const {
        return static_cast<bitType>(i & (BlockTypeOne << b));
    }

 public:
    static const sizeType npos = std::numeric_limits<sizeType>::max();
    explicit Bitset(sizeType bits_);
    Bitset();
    ~Bitset();

    /**
     * Proxy class to simulate lvalues of bit type.
     * Implementation taken from boost dynamic_bitset.
     */
     class BitReference {
         friend class Bitset<BlockType>;

         BitReference(BlockType *b, BlockType pos) :
                 mblock(b),
                 mmask(static_cast<BlockType>(BlockType(1) << pos))
         {}

      private:
         BlockType *mblock;
         const BlockType mmask;

         inline void doSet() { *mblock = *mblock | mmask; }
         inline void doReset() { *mblock = *mblock & static_cast<BlockType>(~mmask); }
         inline void doFlip() { *mblock = *mblock ^ mmask; }
         inline void doAssign(bool b) { b ? doSet() : doReset(); }

      public:
         operator bool() const { return (*mblock & mmask) != 0; }
         bool operator~() const { return (*mblock & mmask) == 0; }

         BitReference& flip() {
             doFlip();
             return *this;
         }

         // for b[i] = x
         BitReference& operator=(bool x) {
             doAssign(x);
             return *this;
         }

         // for b[i] = b[j]
         BitReference& operator=(const BitReference& rhs) {
             doAssign(rhs);
             return *this;
         }

         BitReference& operator|=(bool x) {
             if (x) doSet();
             return *this;
         }

         BitReference& operator&=(bool x) {
             if (!x) doReset();
             return *this;
         }

         BitReference& operator^=(bool x) {
             if (x) doFlip();
             return *this;
         }

         BitReference& operator-=(bool x) {
             if (x) doReset();
             return *this;
         }
     };

     /**
      * non-const version of the [] operator, uses the BitReference wrapper class.
      * @param bit index of the bit
      * @return BitReference referencing the block and index of the bit.
      */
     BitReference operator[](sizeType bit);

     /**
      * const version of the operator needs only a simple get instead of the BitReference wrapper class.
      * @param bit index of the bit
      * @return true if set, false otherwise
      */
     bool operator[](sizeType bit) const;

    /**
     * sets all bits to true
     */
    void set();

    /**
     * clears all bits
     */
    void clear();

    /**
     * flip bitset
     */
    void flip();

    /**
     * @param bit idx of the bit
     * @return true if the bit is set, false otherwise
     */
    bitType get(sizeType bit) const;

    /**
     * @return number of bits held by bitset
     */
    sizeType size() const;
    /**
     * @return number of blocks used to store bits
     */
    sizeType blocks() const;

    /**
     * @param idx of the block
     * @return const ref to the block
     */
    const BlockType& getBlock(sizeType idx) const;

    /**
     * @param idx of the bit the block should start at, it will create a new block starting at idx until idx+blocksize.
     * This way you can get a block that is somewhere in the bitset, but residing in two actual blocks.
     * @return the created new block
     */
    BlockType getShiftedBlock(sizeType idx) const;

    /**
     * @param idx of the block
     * @param block value to be set
     */
    void setBlock(sizeType idx,  BlockType block);

    //  basic bitset operations
    Bitset<BlockType>& operator&=(const Bitset<BlockType>& rhs);

    Bitset<BlockType>& operator|=(const Bitset<BlockType>& rhs);

    Bitset<BlockType>& operator^=(const Bitset<BlockType>& rhs);

    Bitset<BlockType>& operator-=(const Bitset<BlockType>& rhs);

    Bitset<BlockType> operator~() const;

    /**
     * resizes the bitset to hold up to size bits
     * @param size - size of the bitset after resizing
     */
    void resize(unsigned long size);
};

template <typename Block>
Bitset<Block> operator&(const Bitset<Block>& lhs, const Bitset<Block>& rhs) {
    Bitset<Block> b(lhs);
    return b &= rhs;
}

template <typename Block>
Bitset<Block> operator|(const Bitset<Block>& lhs, const Bitset<Block>& rhs) {
    Bitset<Block> b(lhs);
    return b |= rhs;
}

template <typename Block>
Bitset<Block> operator^(const Bitset<Block>& lhs, const Bitset<Block>& rhs) {
    Bitset<Block> b(lhs);
    return b ^= rhs;
}

template <typename Block>
Bitset<Block> operator-(const Bitset<Block>& lhs, const Bitset<Block>& rhs) {
    Bitset<Block> b(lhs);
    return b -= rhs;
}

}  // namespace Sealib
#endif  // SEALIB_BITSET_H_

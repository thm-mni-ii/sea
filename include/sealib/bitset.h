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
class Bitset {
    typedef unsigned long blocktype;
    typedef unsigned long sizetype;
    typedef bool bittype;

 private:
    static const unsigned int bitsPerByte = 64;
    static const blocktype blocktype_one = blocktype(1);

    sizetype bits;
    std::vector<blocktype> mbits;

    inline bittype get(const blocktype &i, sizetype b) const {
        return i & (blocktype_one << b);
    }

    inline void set(blocktype &i, sizetype b) {
        i |= (blocktype_one << b);
    }

    inline void clear(blocktype &i, sizetype b) {
        i &= ~(blocktype_one << b);
    }

    inline void flip(blocktype &i, sizetype b) {
        i ^= (blocktype_one << b);
    }

 public:
    explicit Bitset(sizetype bits_) : bits(bits_), mbits(bits % bitsPerByte == 0 ? bits/bitsPerByte : bits/bitsPerByte + 1) {}
    Bitset() : Bitset(0) {}
    ~Bitset() = default;

    /**
     * Proxy class to simulate lvalues of bit type.
     * Implementation taken from boost dynamic_bitset.
     */
     class BitReference {
         friend class Bitset;

         BitReference(blocktype &b, sizetype pos) :
                 mblock(b),
                 mmask(blocktype(1) << pos)
         {}

      private:
         blocktype &mblock;
         const blocktype mmask;

         void doSet() { mblock |= mmask; }
         void doReset() { mblock &= ~mmask; }
         void doFlip() { mblock ^= mmask; }
         void doAssign(bool b) { b ? doSet() : doReset(); }

      public:
         operator bool() const { return (mblock & mmask) != 0; }
         bool operator~() const { return (mblock & mmask) == 0; }

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
     BitReference operator[](sizetype bit) {
         return BitReference(mbits[bit / bitsPerByte], bit % bitsPerByte);
     }

     /**
      * const version of the operator needs only a simple get instead of the BitReference wrapper class.
      * @param bit index of the bit
      * @return true if set, false otherwise
      */
     bool operator[](sizetype bit) const { return get(bit); }

    /**
     * sets all bits to true
     */
    void set() {
        for (sizetype i = 0; i < mbits.size(); i++) {
            mbits[i] = std::numeric_limits<blocktype>::max();
        }
    }

    /**
     * clears all bits
     */
    void clear() {
        for (sizetype i = 0; i < mbits.size(); i++) {
            mbits[i] = 0;
        }
    }

    /**
     * @param bit idx of the bit
     * @return true if the bit is set, false otherwise
     */
    inline bittype get(sizetype bit) const {
        assert(bit < bits);
        return get(mbits[bit / bitsPerByte], bit % bitsPerByte);
    }

    /**
     * @param bit idx of the bit to be set true
     */
    inline void set(sizetype bit) {
        assert(bit < bits);
        set(mbits[bit / bitsPerByte], bit % bitsPerByte);
    }

    /**
     * @param bit idx of the bit to be set false
     */
    inline void unset(sizetype bit) {
        assert(bit < bits);
        clear(mbits[bit / bitsPerByte], bit % bitsPerByte);
    }

    /**
     * @param bit to be flipped
     */
    inline void flip(sizetype bit) {
        assert(bit < bits);
        flip(mbits[bit / bitsPerByte], bit % bitsPerByte);
    }

    sizetype size() {
        return bits;
    }

    sizetype blocks() {
        return mbits.size();
    }

    /**
     * @param idx of the block
     * @return const ref to the block
     */
    inline const blocktype& getBlock(sizetype idx) const {
        assert(mbits.size() > idx);
        return mbits[idx];
    }

    /**
        * @param idx of the block
        * @param block value to be set
        */
    inline void setBlock(sizetype idx,  blocktype block) {
        assert(mbits.size() > idx);
        mbits[idx] = block;
    }
};
}  // namespace Sealib
#endif  // SEALIB_BITSET_H_

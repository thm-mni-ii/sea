//
// Created by jmeintrup on 30.06.18.
//

#ifndef SEA_BITSET_H
#define SEA_BITSET_H

#include <vector>
#include <cassert>
#include <limits>

namespace Sealib {
    /**
     * Simple Bitset to be used by all classes that need a dynamically allocated bitset.
     * Adds the ability to simply adress whole blocks in the bitset for table lookup convenience.
     * No implementation of the [] operator for convenience, because it adds overhead.
     * @author Johannes Meintrup
     */
    class Bitset {
        typedef unsigned char blocktype;
        typedef unsigned long sizetype;
        typedef bool bittype;
    public:
        explicit Bitset(sizetype bits) : mbits(bits/bitsPerByte + 1), bits(bits) {}
        ~Bitset() = default;

    private:
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

        /**
         * sets all bits to true
         */
        void set() {
            for(sizetype i; i < mbits.size(); i++) {
                mbits[i] = std::numeric_limits<blocktype>::max();
            }
        }

        /**
         * clears all bits
         */
        void clear() {
            for(sizetype i; i < mbits.size(); i++) {
                mbits[i] = 0;
            }
        }

        /**
         * @param bit idx of the bit
         * @return true if the bit is set, false otherwise
         */
        inline bittype get(sizetype bit) {
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

    private:
        static unsigned int bitsPerByte = 8;
        static blocktype blocktype_one = blocktype(1);

        std::vector<blocktype> mbits;
        sizetype bits;

        inline bittype get(const blocktype &i, sizetype b){
            return i & (blocktype_one << b);
        }

        inline void set(blocktype &i, sizetype b){
            i |= (blocktype_one << b);
        }

        inline void clear(blocktype &i, sizetype b){
            i &= ~(blocktype_one << b);
        }

        inline void flip(blocktype &i, sizetype b){
            i ^= (blocktype_one << b);
        }
    };
}
#endif //SEA_BITSET_H

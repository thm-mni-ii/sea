#ifndef SEALIB_COLLECTION_STATICSPACESTORAGE_H_
#define SEALIB_COLLECTION_STATICSPACESTORAGE_H_

#include "sealib/_types.h"
#include "sealib/collection/sequence.h"

namespace Sealib {

/**
 * Array with constant Time initialization using up to
 * 3*64bit extra space compared to std::array
 */
template <uint64_t N>
class ConstantTimeArray {
 public:
    class CTProxy;
    /**
     * Create a new ConstantTimeArray with N elements.
     * @param i initial value of all N entries
     */
    explicit ConstantTimeArray<N>(uint64_t init = 0) : init(init), border(0) {}

    /**
     * Insert a value to the given index.
     * @param i the destination index
     * @param value to insert
     */
    void insert(uint64_t i, uint64_t value);

    /**
     * Get a value from the array.
     * @param _i index to get the value from
     * @return the found value
     */
    uint64_t get(uint64_t i) const;

    /**
     * Resets the complete array to a new initial value.
     * @param _init new value
     */
    void reset(uint64_t _init = 0);

    uint64_t length() { return N; }



    class CTProxy {
     public:
        CTProxy(ConstantTimeArray<N> *ref, uint64_t i) : ref(ref), i(i) {}

        operator uint64_t () {
            return ref->get(i);
        };

        // for a[i] = x
        CTProxy& operator=(uint64_t x) {
            ref->set(i, x);
            return *this;
        };

        CTProxy& operator|=(uint64_t x) {
            ref->set(i, ref->get(i) | x);
            return *this;
        }

        CTProxy& operator&=(uint64_t x) {
            ref->set(i, ref->get(i) & x);
            return *this;
        }

        CTProxy& operator^=(uint64_t x) {
            ref->set(i, ref->get(i) ^ x);
            return *this;
        };

        CTProxy& operator-=(uint64_t x) {
            ref->set(i, ref->get(i) - x);
            return *this;
        };

        CTProxy& operator+=(uint64_t x) {
            ref->set(i, ref->get(i) + x);
            return *this;
        };

        CTProxy& operator*=(uint64_t x) {
            ref->set(i, ref->get(i) * x);
            return *this;
        };

        // for a[i] = b[j]
        CTProxy& operator=(const CTProxy& rhs) {
            return operator=(rhs.ref->get(rhs.i));
        }

        CTProxy& operator|=(const CTProxy& rhs){
            return operator|=(rhs.ref->get(rhs.i));
        };

        CTProxy& operator&=(const CTProxy& rhs){
            return operator&=(rhs.ref->get(rhs.i));
        };

        CTProxy& operator^=(const CTProxy& rhs){
            return operator^=(rhs.ref->get(rhs.i));
        };

        CTProxy& operator-=(const CTProxy& rhs){
            return operator-=(rhs.ref->get(rhs.i));
        };

        CTProxy& operator+=(const CTProxy& rhs){
            return operator+=(rhs.ref->get(rhs.i));
        };

        CTProxy& operator*=(const CTProxy& rhs){
            return operator*=(rhs.ref->get(rhs.i));
        };

     private:
        friend class ConstantTimeArray<N>;
        ConstantTimeArray<N> *ref;
        uint64_t i;
    };

    CTProxy operator[](uint64_t i) { return CTProxy(this, i); }
    uint64_t operator[](uint64_t i) const { return get(i); }

 private:
    std::array<uint64_t, N % 2 == 0 ? N : N + 1> A;
    uint64_t init;
    uint64_t border;

    bool isChain(uint64_t i);
    uint64_t chainWith(uint64_t i);
    void makeChain(uint64_t i, uint64_t j);
    void breakChain(uint64_t i);
    void initBlock(uint64_t i);
    uint64_t extend();

    friend class CTProxy;
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_STATICSPACESTORAGE_H_

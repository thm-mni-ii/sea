#ifndef SEALIB_COLLECTION_INITIALIZEDARRAY_H_
#define SEALIB_COLLECTION_INITIALIZEDARRAY_H_

#include <memory>
#include "sealib/_types.h"
#include "sealib/collection/sequence.h"

namespace Sealib {

/**
 * An array of initialized fields. This array is initialized in constant time by using
 * O(1) computer words in addition to the memory for an standard C array.
 * Read and write operation run in constant time.
 */
class InitializedArray {
 public:
    class proxy;
    class iterator;
    /**
     * Initializes an array with a user defined element in constant time.
     * @param n The number of fields inside the initialized array
     * @param init Initial value of all n entries
     */
    explicit InitializedArray(uint64_t n = 0, uint64_t init = 0);

    /**
     * Reset all fields of the initialized array to a new value.
     * @param _init new value
     */
    void reset(uint64_t _init = 0) { init = _init, border = 0; }

    uint64_t size() const { return _size; }

    /**
     * Set and get values of the array via this.
     * Works like any normal array for the user.
     * Internally uses a proxy class to distinguish
     * between setting and getting. I.e., lvalue and rvalue distinction.
     * @param i index of the value
     * @return returns the value at index i
     */
    proxy operator[](uint64_t i) { return {this, i}; }
    uint64_t operator[](uint64_t i) const { return get(i); }

    class proxy {
     public:
        proxy(InitializedArray* ref, uint64_t i) : ref(ref), i(i) {}

        operator uint64_t() const { return ref->get(i); }

        proxy& operator=(uint64_t x) {
            ref->insert(i, x);
            return *this;
        }

        proxy& operator|=(uint64_t x) {
            ref->insert(i, ref->get(i) | x);
            return *this;
        }

        proxy& operator&=(uint64_t x) {
            ref->insert(i, ref->get(i) & x);
            return *this;
        }

        proxy& operator^=(uint64_t x) {
            ref->insert(i, ref->get(i) ^ x);
            return *this;
        }

        proxy& operator-=(uint64_t x) {
            ref->insert(i, ref->get(i) - x);
            return *this;
        }

        proxy& operator+=(uint64_t x) {
            ref->insert(i, ref->get(i) + x);
            return *this;
        }

        proxy& operator*=(uint64_t x) {
            ref->insert(i, ref->get(i) * x);
            return *this;
        }

     private:
        friend class InitializedArray;
        InitializedArray* ref;
        uint64_t i;
    };

 private:
    friend class InitializedArray::proxy;

    uint64_t _size;
    uint64_t init;
    uint64_t border;
    uint64_t _sizeA;
    std::unique_ptr<uint64_t[]> A;

    bool isChain(uint64_t i) const;
    uint64_t chainWith(uint64_t i);
    void makeChain(uint64_t i, uint64_t j);
    void breakChain(uint64_t i);
    void initBlock(uint64_t i);
    uint64_t extend();
    void insert(uint64_t i, uint64_t value);

    uint64_t get(uint64_t i) const;
};
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_INITIALIZEDARRAY_H_

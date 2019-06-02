#ifndef SEALIB_COLLECTION_CONSTANTTIMEDYNAMICARRAY_H_
#define SEALIB_COLLECTION_CONSTANTTIMEDYNAMICARRAY_H_

#include "sealib/_types.h"
#include "sealib/collection/sequence.h"

namespace Sealib {

/**
 * Dynamic Array with constant Time initialization using O(1) bytes extra space
 */
class ConstantTimeDynamicArray {
 public:
    class proxy;
    class iterator;
    /**
     * Create a new ConstantTimeArray with N elements.
     * @param init initial value of all N entries
     */
    explicit ConstantTimeDynamicArray(uint64_t size = 0, uint64_t init = 0)
        : _size(size),
          init(init),
          border(0),
          A(new uint64_t[(size % 2 ? size : size + 1)]) {}

    /**
     * Fills the array with the specified value in constant time.
     * @param _init new value
     */
    void fill(uint64_t _init = 0) { init = _init, border = 0; }

    uint64_t size() const { return _size; }

    /**
     * Set and get values of the array via this.
     * Works like any normal array for the user.
     * Internally uses a proxy class to distinguish
     * between setting and getting. I.e., lvalue and rvalue distinction.
     * @param i index of the value
     * @return returns the value at index i
     */
    proxy operator[](uint64_t i) { return proxy(this, i); }
    uint64_t operator[](uint64_t i) const { return get(i); }

    void insert(uint64_t i, uint64_t value);
    uint64_t get(uint64_t i) const;

    class proxy {
     public:
        proxy(ConstantTimeDynamicArray* ref, uint64_t i) : ref(ref), i(i) {}

        operator uint64_t() const { return ref->get(i); }

        // for a[i] = x
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

        // for a[i] = b[j]
        proxy& operator=(const proxy& rhs) {
            return operator=(rhs.ref->get(rhs.i));
        }

        proxy& operator|=(const proxy& rhs) {
            return operator|=(rhs.ref->get(rhs.i));
        }

        proxy& operator&=(const proxy& rhs) {
            return operator&=(rhs.ref->get(rhs.i));
        }

        proxy& operator^=(const proxy& rhs) {
            return operator^=(rhs.ref->get(rhs.i));
        }

        proxy& operator-=(const proxy& rhs) {
            return operator-=(rhs.ref->get(rhs.i));
        }

        proxy& operator+=(const proxy& rhs) {
            return operator+=(rhs.ref->get(rhs.i));
        }

        proxy& operator*=(const proxy& rhs) {
            return operator*=(rhs.ref->get(rhs.i));
        }

     private:
        friend class ConstantTimeDynamicArray;
        ConstantTimeDynamicArray* ref;
        uint64_t i;
    };

 private:
    uint64_t _size;
    uint64_t init;
    uint64_t border;
    std::unique_ptr<uint64_t[]> A;

    bool isChain(uint64_t i) const;
    uint64_t chainWith(uint64_t i);
    void makeChain(uint64_t i, uint64_t j);
    void breakChain(uint64_t i);
    void initBlock(uint64_t i);
    uint64_t extend();

    friend class proxy;
};
struct NotChained : public std::exception {
    const char* what() const noexcept override { return "NotChained"; }
};

void ConstantTimeDynamicArray::insert(uint64_t i, uint64_t value) {
    uint64_t _i = i / 2;
    if (_i < border) {
        try {
            uint64_t k = chainWith(_i);
            uint64_t j = extend();
            if (_i == j) {
                A[i] = value;
                breakChain(_i);
            } else {
                A[2 * j] = A[2 * _i];
                A[2 * j + 1] = A[2 * _i + 1];
                makeChain(j, k);
                initBlock(_i);
                A[i] = value;
                breakChain(_i);
            }
        } catch (NotChained& e) {
            A[i] = value;
            breakChain(_i);
        }
    } else {
        try {
            uint64_t k = chainWith(_i);
            if (i % 2 == 0) {
                A[2 * k + 1] = value;
            } else {
                A[i] = value;
            }
        } catch (NotChained& e) {
            uint64_t k = extend();
            if (_i == k) {
                A[i] = value;
                breakChain(_i);
            } else {
                initBlock(_i);
                makeChain(k, _i);
                if (i % 2 == 0) {
                    A[2 * k + 1] = value;
                } else {
                    A[i] = value;
                }
            }
        }
    }
}

uint64_t ConstantTimeDynamicArray::get(uint64_t i) const {
    uint64_t _i = i / 2;
    if (isChain(_i)) {
        if (i < 2 * border) {
            return init;
        } else {
            if (i % 2 == 0) {
                return A[A[i] + 1];
            } else {
                return A[i];
            }
        }
    } else {
        return (i < 2 * border) ? A[i] : init;
    }
}

uint64_t ConstantTimeDynamicArray::chainWith(uint64_t i) {
    uint64_t _k = A[2 * i];
    uint64_t k = _k / 2;
    if (isChain(i)) {
        return k;
    } else {
        throw NotChained();
    }
}

void ConstantTimeDynamicArray::makeChain(uint64_t i, uint64_t j) {
    A[2 * i] = 2 * j;
    A[2 * j] = 2 * i;
}

void ConstantTimeDynamicArray::breakChain(uint64_t i) {
    try {
        uint64_t k = chainWith(i);
        A[2 * k] = 2 * k;
    } catch (NotChained& e) {
        // not chained, nothing to break
    }
}

void ConstantTimeDynamicArray::initBlock(uint64_t i) {
    A[2 * i] = A[2 * i + 1] = init;
}

uint64_t ConstantTimeDynamicArray::extend() {
    uint64_t k;
    try {
        k = chainWith(border);
        border++;
        A[2 * (border - 1)] = A[2 * k + 1];
        breakChain(border - 1);
    } catch (NotChained& e) {
        k = border++;
    }
    initBlock(k);
    breakChain(k);
    return k;
}

bool ConstantTimeDynamicArray::isChain(uint64_t i) const {
    uint64_t _k = A[2 * i];
    uint64_t k = _k / 2;
    return (_k % 2 == 0 && _k >= 0 && _k < _size && A[_k] == 2 * i &&
            ((i < border && border <= k) || (k < border && border <= i)));
}
}  // namespace Sealib
#endif  // SEALIB_COLLECTION_CONSTANTTIMEDYNAMICARRAY_H_

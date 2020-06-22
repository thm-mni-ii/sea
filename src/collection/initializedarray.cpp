#include <sealib/collection/initializedarray.h>

#include <stdexcept>

using Sealib::InitializedArray;

struct NotChained : public std::exception {
    const char* what() const noexcept override { return "NotChained"; }
};

void InitializedArray::insert(uint64_t i, uint64_t value) {
    if (i >= _size) throw std::out_of_range("Index out of bounds.");
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

uint64_t InitializedArray::get(uint64_t i) const {
    if (i >= _size) throw std::out_of_range("Index out of bounds.");
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

uint64_t InitializedArray::chainWith(uint64_t i) {
    uint64_t _k = A[2 * i];
    uint64_t k = _k / 2;
    if (isChain(i)) {
        return k;
    } else {
        throw NotChained();
    }
}

void InitializedArray::makeChain(uint64_t i, uint64_t j) {
    A[2 * i] = 2 * j;
    A[2 * j] = 2 * i;
}

void InitializedArray::breakChain(uint64_t i) {
    try {
        uint64_t k = chainWith(i);
        A[2 * k] = 2 * k;
    } catch (NotChained& e) {
        // not chained, nothing to break
    }
}

void InitializedArray::initBlock(uint64_t i) {
    A[2 * i] = A[2 * i + 1] = init;
}

uint64_t InitializedArray::extend() {
    uint64_t k;
    try {
        k = chainWith(border);
        A[2 * border] = A[2 * k + 1];
        breakChain(border);
        border++;
    } catch (NotChained& e) {
        k = border++;
    }
    initBlock(k);
    breakChain(k);
    return k;
}

bool InitializedArray::isChain(uint64_t i) const {
    uint64_t _k = A[2 * i];
    uint64_t k = _k / 2;
    return (_k % 2 == 0 && _k < _sizeA && A[_k] == 2 * i &&
            ((i < border && border <= k) || (k < border && border <= i)));
}
Sealib::InitializedArray::InitializedArray(uint64_t size,
                                           uint64_t init)
    : _size(size),
      init(init),
      border(0),
      _sizeA((size % 2 == 0 ? size : size + 1)),
      A(new uint64_t[(size % 2 == 0 ? size : size + 1)]) {}

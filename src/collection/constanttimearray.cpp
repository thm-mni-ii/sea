#include <sealib/collection/constanttimearray.h>

 struct NotChained : public std::exception {
     const char * what () const noexcept override
     {
         return "NotChained";
     }
};


template <uint64_t N>
void Sealib::ConstantTimeArray<N>::insert(uint64_t i, uint64_t value) {
    uint64_t _i = i / 2;
    if (_i < border) {
        try {
            uint64_t k = chainWith(_i);
            uint64_t j = extend();
            if (_i == j) {
                A[i] = value;
                breakChain(_i);
            } else {
                setA(2 * j, A[2 * _i].get());
                setA(2 * j + 1, A[2 * _i + 1].get());
                makeChain(j, k);
                initBlock(_i);
                A[i] = value;
                breakChain(_i);
            }
        } catch (NotChained &e) {
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
        } catch (NotChained &e) {
            uint64_t k = extend();
            if (_i == k) {
                A[i, value];
                breakChain(_i);
            } else {
                initBlock(_i);
                makeChain(k, _i);
                if (i % 2 == 0) {
                    A[2*k + 1] = value;
                } else {
                    A[i, value];
                }
            }
        }
    }
}

template <uint64_t N>
uint64_t Sealib::ConstantTimeArray<N>::get(uint64_t i) const {
    uint64_t _i = i / 2;
    if(isChain(_i)) {
        if (i < 2 * border) {
            return init;
        } else {
            if (i % 2 == 0) {
                return A[A[i] + 1];
            } else {
                return A[i];
            }
        }
    } else  {
        return (i < 2 * border) ? A[i].get() : init;
    }
}

template <uint64_t N>
uint64_t Sealib::ConstantTimeArray<N>::chainWith(uint64_t i) {
    uint64_t _k = A[2 * i];
    uint64_t k = _k / 2;
    if (isChain(i)) {
        return k;
    } else {
        throw NotChained();
    }
}

template <uint64_t N>
void Sealib::ConstantTimeArray<N>::makeChain(uint64_t i, uint64_t j) {
    A[2*i]=2*j;
    A[2*j]=2*i;
}

template <uint64_t N>
void Sealib::ConstantTimeArray<N>::breakChain(uint64_t i) {
    try {
        uint64_t k = chainWith(i);
        A[2*k]=2*k;
    } catch (NotChained &e) {
        // not chained, nothing to break
    }
}

template <uint64_t N>
void Sealib::ConstantTimeArray<N>::initBlock(uint64_t i) {
    A[2 * i] = A[2 * i + 1] = init;
}

template <uint64_t N>
uint64_t Sealib::ConstantTimeArray<N>::extend() {
    uint64_t k;
    try {
        k = chainWith(border);
        A[2*(border - 1)]= A[2*k + 1];
        breakChain(border-1); }
    catch (NotChained &e) {
        k = border - 1;
    }
    initBlock(k);
    breakChain(k);
    return k;
}

template <uint64_t N>
void Sealib::ConstantTimeArray<N>::reset(uint64_t _init) { init = _init, border = 0; }
template <uint64_t N>

bool Sealib::ConstantTimeArray<N>::isChain(uint64_t i) {
    uint64_t _k = A[2 * i];
    uint64_t k = _k / 2;
    return  (_k % 2 == 0 && _k >= 0
        && _k < A.length && A[_k].get() == 2 * i
        && ((i < border && border <= k) || (k < border && border <= i)));
}

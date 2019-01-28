#ifndef SRC_COLLECTION_SIMPLESEQUENCE_H_
#define SRC_COLLECTION_SIMPLESEQUENCE_H_
#include <vector>
#include "sealib/collection/sequence.h"

namespace Sealib {
/**
 * Simple sequence implementation, useful for debugging.
 */
template <class T>
class SimpleSequence : public Sequence<T> {
 public:
    T get(uint i) const override { return data[i]; }

    void insert(uint i, T v) override { data[i] = v; }

    explicit SimpleSequence(uint size) : data(size) {}

 private:
    std::vector<T> data;
};
}  // namespace Sealib
#endif  // SRC_COLLECTION_SIMPLESEQUENCE_H_

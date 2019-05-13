#ifndef SRC_COLLECTION_SIMPLESEQUENCE_H_
#define SRC_COLLECTION_SIMPLESEQUENCE_H_
#include <vector>
#include "sealib/collection/sequence.h"

namespace Sealib {
/**
 * Simple sequence implementation, useful for debugging.
 * @tparam T type of elements held by the internal vector
 */
template <class T>
class SimpleSequence : public Sequence<uint64_t> {
 public:
    uint64_t get(uint64_t i) const override { return data[i]; }

    void insert(uint64_t i, uint64_t v) override {
        data[i] = static_cast<T>(v);
    }

    explicit SimpleSequence(uint64_t size) : data(size) {}

    uint64_t byteSize() const { return data.capacity() * sizeof(T); }

 private:
    std::vector<T> data;
};
}  // namespace Sealib
#endif  // SRC_COLLECTION_SIMPLESEQUENCE_H_

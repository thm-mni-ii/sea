#ifndef SRC_SIMPLECONTAINER_H_
#define SRC_SIMPLECONTAINER_H_
#include <vector>
#include "sealib/container.h"

namespace Sealib {
/**
 * Simple container implementation, useful for debugging.
 */
template <class T>
class SimpleContainer : public Container<T> {
 public:
    T get(uint i) const override { return data[i]; }

    void insert(uint i, T v) override { data[i] = v; }

    explicit SimpleContainer(uint size) : data(size) {}

 private:
    std::vector<T> data;
};
}  // namespace Sealib
#endif  // SRC_SIMPLECONTAINER_H_

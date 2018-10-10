#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <functional>
namespace Sealib {
// the standard numeric type (used mainly for node names)
typedef unsigned int uint;

typedef std::function<void(uint)> UserFunc1;
typedef std::function<void(uint, uint)> UserFunc2;

static constexpr unsigned int INVALID = static_cast<unsigned>(-1);

// a pair of numbers (used mainly for stack management)
// typedef std::tuple<uint, uint> Pair;
class Pair {
 private:
  uint v1, v2;

 public:
  Pair() : v1(), v2() {}
  Pair(uint a, uint b) : v1(a), v2(b) {}
  uint head() const { return v1; }
  uint tail() const { return v2; }
  bool operator==(const Pair &x) {
    return x.head() == v1 && x.tail() == v2;
  }
  bool operator!=(const Pair &x) {
    return x.head() != v1 || x.tail() != v2;
  }
};
} // namespace Sealib
#endif  // SEALIB__TYPES_H_

#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <functional>
#include <cstdint>

namespace Sealib {
// the standard numeric type (used mainly for node names)
typedef unsigned int uint;

typedef std::function<void(uint)> UserFunc1;
typedef std::function<void(uint, uint)> UserFunc2;

static constexpr uint INVALID = static_cast<uint>(-1);

// a pair of numbers (used mainly for stack management)
class Pair {
 private:
  uint v1, v2;

 public:
  constexpr Pair() : v1(), v2() {}
  constexpr Pair(uint a, uint b) : v1(a), v2(b) {}
  constexpr uint head() const { return v1; }
  constexpr uint tail() const { return v2; }
  bool operator==(const Pair &x) {
    return x.head() == v1 && x.tail() == v2;
  }
  bool operator!=(const Pair &x) {
    return x.head() != v1 || x.tail() != v2;
  }
};
}  // namespace Sealib

/**
 * Macro for constexpr qualifier on member functions
 */
#ifdef __clang__
#define CONSTEXPR_IF_CLANG constexpr
#else
#define CONSTEXPR_IF_CLANG
#endif

#endif  // SEALIB__TYPES_H_

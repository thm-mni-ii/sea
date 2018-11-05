#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <cstdint>

// the standard numeric type (used mainly for node names)
typedef uint32_t uint;

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
};

/**
 * Macro for constexpr qualifier on member functions
 */
#ifdef __clang__
#define CONSTEXPR_IF_CLANG constexpr
#else
#define CONSTEXPR_IF_CLANG
#endif

#endif  // SEALIB__TYPES_H_

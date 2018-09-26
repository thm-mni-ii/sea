#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_

// the standard numeric type (used mainly for node names)
typedef unsigned int uint;

// a pair of numbers (used mainly for stack management)
// typedef std::tuple<uint, uint> Pair;
class Pair {
 private:
  uint v1, v2;

 public:
  Pair() : v1(), v2() {}
  Pair(uint a, uint b) : v1(a), v2(b) {}
  uint head() { return v1; }
  uint tail() { return v2; }
  bool operator==(Pair &x) {
    return x.head()==v1 && x.tail()==v2;
  }
};

#endif  // SEALIB__TYPES_H_

#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <tuple>

// the standard numeric type (used mainly for node names)
typedef unsigned int uint;

// a pair of numbers (used mainly for stack management)
typedef std::tuple<uint, uint> Pair;

#endif  // SEALIB__TYPES_H_

#include "sealib/graph/adjacency.h"
#include <limits>

using Sealib::Adjacency;

Adjacency::Adjacency(uint32_t _vertex) :
    vertex(_vertex),
    crossIndex(std::numeric_limits<uint32_t>::max()) {
}

Adjacency::Adjacency() :
    vertex(std::numeric_limits<uint32_t>::max()),
    crossIndex(std::numeric_limits<uint32_t>::max()) {}

#include "sealib/adjacency.h"
#include <limits>

using Sealib::Adjacency;

Adjacency::Adjacency(unsigned int _vertex) :
    vertex(_vertex),
    crossIndex(std::numeric_limits<unsigned int>::max()) {
}

Adjacency::Adjacency() :
    vertex(std::numeric_limits<unsigned int>::max()),
    crossIndex(std::numeric_limits<unsigned int>::max()) {}

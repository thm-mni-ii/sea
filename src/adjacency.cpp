#include "sealib/adjacency.h"
#include <limits>
#include <functional>

using Sealib::Adjacency;

Adjacency::Adjacency(unsigned int _vertex) :
    vertex(_vertex),
    crossIndex(std::numeric_limits<unsigned int>::max()) {
}

Adjacency::Adjacency() :
    vertex(std::numeric_limits<unsigned int>::max()),
    crossIndex(std::numeric_limits<unsigned int>::max()) {}

std::tuple<unsigned int, unsigned int> Sealib::Adjacency::toTuple() const {
    return std::make_tuple(std::ref(vertex), std::ref(crossIndex));
}

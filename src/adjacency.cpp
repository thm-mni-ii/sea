#include "sealib/adjacency.h"
#include <limits>


Sealib::Adjacency::Adjacency(unsigned int _vertex) : vertex(_vertex) {
    crossIndex = std::numeric_limits<unsigned int>::max();
}

Sealib::Adjacency::Adjacency() {

}

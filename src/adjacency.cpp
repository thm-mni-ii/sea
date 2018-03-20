//
// Created by jmeintrup on 13.03.18.
//
#include "sealib/adjacency.h"
#include <limits>

/**
 * Adjacency constructor.
 * Vertex is the number of the vertex
 * Index has to be set after the other vertex to be crosslinked with has been created.
 * It is initialized with unsigned int max value, which serves as a check for unlinked.
 * This means that it is only possible to create max(unsigned int) - 1 edges for a node.
 * @param vertex
 */
Adjacency::Adjacency(size_t _vertex) :vertex(_vertex){
    crossIndex = std::numeric_limits<size_t>::max();
}

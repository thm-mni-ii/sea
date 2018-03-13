//
// Created by jmeintrup on 13.03.18.
//
#include "sealib/adjacency.h"
#include <limits>

/**
     * Adjacency constructor.
     * Vertex is the number of the vertex
     * Index has to be set after the other vertex to be crosslinked with has been created
     * @param vertex
     */

Adjacency::Adjacency(unsigned int _vertex) :vertex(_vertex){
    index = std::numeric_limits<unsigned int>::max();
}

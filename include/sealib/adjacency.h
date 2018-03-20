//
// Created by jmeintrup on 12.03.18.
//

#ifndef SEA_ADJACENCY_H
#define SEA_ADJACENCY_H

#include <cstddef>

/**
 * Adjacency class.
 * An Adjacency object is used in the nodes class as a representation of an edges endpoint
 * and a crossIndex which serves as a crosslink to it's matched edge.
 */
class Adjacency {
public:
    size_t vertex;
    size_t crossIndex;

    /**
     * Adjacency constructor.
     * Vertex is the number of the vertex
     * Index has to be set after the other vertex to be crosslinked with has been created.
     * It is initialized with unsigned int max value, which serves as a check for unlinked.
     * This means that it is only possible to create max(unsigned int) - 1 edges for a node.
     * @param vertex
     */
    Adjacency(size_t _vertex);
};



#endif //SEA_ADJACENCY_H

//
// Created by jmeintrup on 12.03.18.
//

#ifndef SEA_ADJACENCY_H
#define SEA_ADJACENCY_H
/**
 * Adjacency class.
 * An Adjacency object is used in the nodes class as a representation of an edges endpoint and a crossIndex which serves
 * as a crosslink to it's matched edge.
 */
class Adjacency {
public:
    unsigned int vertex;
    unsigned int crossIndex;
    /**
     * Adjacency constructor.
     * Vertex is the number of the vertex
     * Index has to be set after the other vertex to be crosslinked with has been created
     * @param vertex
     */
    Adjacency(unsigned int _vertex);
};



#endif //SEA_ADJACENCY_H

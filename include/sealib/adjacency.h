#ifndef SEA_ADJACENCY_H
#define SEA_ADJACENCY_H

/**
 * An Adjacency object is used in the nodes class as a representation of an edges endpoint.
 * Also it contains a cross link to its incomming edge.
 * @author Johannes Meintrup
 */
class Adjacency {
 public:
    unsigned int vertex;
    unsigned int crossIndex;

    /**
     * Creates an adjacency with the given endpoint _vertex.
     * Note: The construction will leave the crossIndex uninitialized.
     * @param _vertex The vertex id
     */
    Adjacency(unsigned int _vertex);
};

#endif //SEA_ADJACENCY_H

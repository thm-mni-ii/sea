#ifndef SEA_ADJACENCY_H
#define SEA_ADJACENCY_H

/**
 * An Adjacency object is used in the nodes class as a representation of an edges endpoint.
 * Also it contains a cross link to its incomming edge.
 * @author Johannes Meintrup
 */
namespace Sealib {
class Adjacency {
 public:
    unsigned int vertex;
    unsigned int crossIndex;

    /**
     * Creates an adjacency with the given endpoint _vertex.
     * Note: The construction will leave the crossIndex uninitialized.
     * @param _vertex The vertex id
     */
    explicit Adjacency(unsigned int _vertex);

    Adjacency();
};
}  // namespace Sealib


#endif  // SEA_ADJACENCY_H

#ifndef SEALIB_GRAPH_ADJACENCY_H_
#define SEALIB_GRAPH_ADJACENCY_H_
#include <cstdint>

namespace Sealib {
/**
 * An Adjacency object is used in the nodes class as a representation of an edges endpoint.
 * Also it contains a cross link to its incomming edge.
 * @author Johannes Meintrup
 */
class Adjacency {
 public:
    uint32_t vertex;
    uint32_t crossIndex;

    /**
     * Creates an adjacency with the given endpoint _vertex.
     * Note: The construction will leave the crossIndex uninitialized.
     * @param _vertex The vertex id
     */
    explicit Adjacency(uint32_t _vertex);

    Adjacency();
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_ADJACENCY_H_

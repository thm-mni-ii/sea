#ifndef SEALIB_ADJACENCY_H_
#define SEALIB_ADJACENCY_H_

#include <tuple>
namespace Sealib {
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
    explicit Adjacency(unsigned int _vertex);

    Adjacency();

    std::tuple<unsigned int, unsigned int> toTuple() const;
};
}  // namespace Sealib
#endif  // SEALIB_ADJACENCY_H_

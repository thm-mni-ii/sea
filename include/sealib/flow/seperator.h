#ifndef SEALIB_FLOW_SEPERATOR_H_
#define SEALIB_FLOW_SEPERATOR_H_

#include <sealib/collection/bitset.h>
#include <sealib/collection/segmentstack.h>
#include <sealib/iterator/iterator.h>
#include "sealib/graph/directedgraph.h"
#include "sealib/graph/undirectedgraph.h"

namespace Sealib {
/**
 * VSeperator is a class that seperates two Sets in a Graph
 * trough a minimum number of vertices.
 * @Author Vytautas Hermann
 */
class Seperator {
 public:
    /**
     * Returns a Set of Nodes that are part of the Seperator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the Graph
     * @param k max vertices the seperator is allowed to have
     * @return Set of vertices, wich disjoin s and t.
     */
    static Sealib::Bitset<> standardVSeperate(Sealib::Bitset<> s,
                                              Sealib::Bitset<> t,
                                              Sealib::Graph const &g,
                                              int64_t k);
    /**
     * Returns a Set of Nodes that are part of the Seperator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the Graph
     * @param k max vertices the seperator is allowed to have
     * @param iter dfs funktion wich returns an iterator
     * @return Set of vertices, wich disjoin s and t.
     */
    static Sealib::Bitset<> standardVSeperate(
        Sealib::Bitset<> s, Sealib::Bitset<> t, Sealib::Graph const &g,
        int64_t k, Iterator<UserCall> *iter(Graph const &, uint64_t));
    /**
     * Returns a vector of edges that are part of the Seperator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the Graph
     * @param k max edges the seperator is allowed to have
     * @return Set of vertices, wich disjoin s and t.
     */
    static std::vector<std::pair<uint64_t, uint64_t>> standardESeperate(
        Sealib::Bitset<> s, Sealib::Bitset<> t, Sealib::Graph const &g,
        int64_t k);
    /**
     * Returns a vector of edges that are part of the Seperator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the Graph
     * @param k max vertices the seperator is allowed to have
     * @param iter dfs funktion wich returns an iterator
     * @return Set of vertices, wich disjoin s and t.
     */
    static std::vector<std::pair<uint64_t, uint64_t>> standardESeperate(
        Sealib::Bitset<> s, Sealib::Bitset<> t, Sealib::Graph const &g,
        int64_t k, Iterator<UserCall> *iter(Graph const &, uint64_t));
};

}  // namespace Sealib

#endif  // SEALIB_FLOW_SEPERATOR_H_

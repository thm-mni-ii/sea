#ifndef SEALIB_FLOW_SEPARATOR_H_
#define SEALIB_FLOW_SEPARATOR_H_

#include <sealib/collection/bitset.h>
#include <sealib/collection/segmentstack.h>
#include <sealib/iterator/iterator.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/undirectedgraph.h>

namespace Sealib {
/**
 * VSeparator is a class that separates two sets in a graph
 * trough a minimum number of vertices.
 * @author Vytautas Hermann
 */
class Separator {
 public:
    /**
     * Returns a set of nodes that are part of the separator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the graph
     * @param k max vertices the separator is allowed to have
     * @return Set of vertices, which, if removed, disconnect s and t.
     */
    static Sealib::Bitset<> standardVSeparate(Sealib::Bitset<> const &s,
                                              Sealib::Bitset<> const &t,
                                              Sealib::Graph const &g,
                                              int64_t k);
    /**
     * Returns a set of nodes that are part of the separator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the graph
     * @param k max vertices the separator is allowed to have
     * @param iter dfs function that returns an iterator
     * @return Set of vertices, which, if removed, disconnect s and t.
     */
    static Sealib::Bitset<> standardVSeparate(
        Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
        Sealib::Graph const &g, int64_t k,
        Iterator<UserCall> *iter(Graph const &, uint64_t));
    static Sealib::Bitset<> vSeparate(
            Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
            Sealib::Graph const &g, int64_t k,
            Iterator<UserCall> *iter(Graph const &, uint64_t));
    /**
     * Returns a vector of edges that are part of the separator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the graph
     * @param k max edges the separator is allowed to have
     * @return Set of vertices, which, if removed, disconnect s and t.
     */
    static std::vector<std::pair<uint64_t, uint64_t>> standardESeparate(
        Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
        Sealib::Graph const &g, int64_t k);
    /**
     * Returns a vector of edges that are part of the separator.
     * @param s first set of vertices
     * @param t second set of vertices
     * @param g the graph
     * @param k max vertices the separator is allowed to have
     * @param iter dfs function that returns an iterator
     * @return Set of vertices, which, if removed, disconnect s and t.
     */
    static std::vector<std::pair<uint64_t, uint64_t>> standardESeparate(
        Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
        Sealib::Graph const &g, int64_t k,
        Iterator<UserCall> *iter(Graph const &, uint64_t));
    static std::vector<std::pair<uint64_t, uint64_t>> eSeparate(
            Sealib::Bitset<> const &s, Sealib::Bitset<> const &t,
            Sealib::Graph const &g, int64_t k,
            Iterator<UserCall> *iter(Graph const &, uint64_t));
};

}  // namespace Sealib

#endif  // SEALIB_FLOW_SEPARATOR_H_

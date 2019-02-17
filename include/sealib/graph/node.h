#ifndef SEALIB_GRAPH_NODE_H_
#define SEALIB_GRAPH_NODE_H_

#include <cstdint>
#include <vector>

namespace Sealib {
/**
 * A Node object containing the nodes adjacencies and its degree.
 * @author Johannes Meintrup
 */
template <class T>
class Node {
 public:
    Node() : adj(0) {}
    explicit Node(std::vector<T> const &p) : adj(p) {}
    uint64_t getDegree() const { return static_cast<uint64_t>(adj.size()); }
    std::vector<T> const &getAdj() const { return adj; }
    std::vector<T> &getAdj() { return adj; }
    void addAdjacency(T const &v) { adj.emplace_back(v); }
 private:
    std::vector<T> adj;
};

/**
 * Simple node that only stores numbers.
 */
typedef Node<uint64_t> SimpleNode;
/**
 * Extended node that also stores cross indices.
 */
typedef Node<std::pair<uint64_t, uint64_t>> ExtendedNode;
}  // namespace Sealib
#endif  // SEALIB_GRAPH_NODE_H_

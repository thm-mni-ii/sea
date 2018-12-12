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
    uint getDegree() const { return adj.size(); }
    std::vector<T> const &getAdj() const { return adj; }
    std::vector<T> &getAdj() { return adj; }
    void addAdjacency(T const &v) { adj.emplace_back(v); }
 private:
    std::vector<T> adj;
};

typedef Node<uint> NodeD;
typedef Node<std::pair<uint, uint>> NodeU;
}  // namespace Sealib
#endif  // SEALIB_GRAPH_NODE_H_

#include <sealib/graph/undirectedgraph.h>

using Sealib::UndirectedGraph;
using Sealib::Node;

Sealib::UndirectedGraph::UndirectedGraph(Node *nodes_, uint32_t order_) : nodes(order_) {
    for (uint32_t i = 0; i < order_; i++) {
        nodes[i] = nodes_[i];
    }
}

Sealib::UndirectedGraph::UndirectedGraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_) {}

Sealib::UndirectedGraph::UndirectedGraph() : nodes(0) {}

Sealib::UndirectedGraph::UndirectedGraph(uint32_t order) : nodes(order) {}

void Sealib::UndirectedGraph::addNode(const Sealib::Node &node) {
    nodes.emplace_back(node);
}
const Node &Sealib::UndirectedGraph::getNode(uint32_t u) const {
    return nodes[u];
}
Node &Sealib::UndirectedGraph::getNode(uint32_t u) {
    return nodes[u];
}
uint32_t Sealib::UndirectedGraph::getNodeDegree(uint32_t u) const {
    return nodes[u].getDegree();
}
uint32_t Sealib::UndirectedGraph::head(uint32_t u, uint32_t k) const {
    return nodes[u].getAdj()[k].vertex;
}
uint32_t Sealib::UndirectedGraph::getOrder() const {
    return static_cast<uint32_t>(nodes.size());
}
std::tuple<uint32_t, uint32_t>
Sealib::UndirectedGraph::mate(uint32_t u, uint32_t k) const {
    return std::tuple<uint32_t, uint32_t>(nodes[u].getAdj()[k].vertex,
                                                  nodes[u].getAdj()[k].crossIndex);
}

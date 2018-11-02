#include <sealib/graph/basicgraph.h>

using Sealib::BasicGraph;
using Sealib::Node;

Sealib::BasicGraph::BasicGraph(Node *nodes_, uint32_t order_) : nodes(order_) {
    for (uint32_t i = 0; i < order_; i++) {
        nodes[i] = nodes_[i];
    }
}

Sealib::BasicGraph::BasicGraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_) {}

Sealib::BasicGraph::BasicGraph() : nodes(0) {}

Sealib::BasicGraph::BasicGraph(uint32_t order) : nodes(order) {}

void Sealib::BasicGraph::addNode(const Sealib::Node &node) {
    nodes.emplace_back(node);
}
const Node &Sealib::BasicGraph::getNode(uint32_t u) const {
    return nodes[u];
}
Node &Sealib::BasicGraph::getNode(uint32_t u) {
    return nodes[u];
}
uint32_t Sealib::BasicGraph::getNodeDegree(uint32_t u) const {
    return nodes[u].getDegree();
}
uint32_t Sealib::BasicGraph::head(uint32_t u, uint32_t k) const {
    return nodes[u].getAdj()[k].vertex;
}
uint32_t Sealib::BasicGraph::getOrder() const {
    return static_cast<uint32_t>(nodes.size());
}
std::tuple<uint32_t, uint32_t>
Sealib::BasicGraph::mate(uint32_t u, uint32_t k) const {
    return std::tuple<uint32_t, uint32_t>(nodes[u].getAdj()[k].vertex,
                                                  nodes[u].getAdj()[k].crossIndex);
}

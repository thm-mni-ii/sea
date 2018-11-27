#include <sealib/graph/directedgraph.h>

using Sealib::DirectedGraph;
using Sealib::Node;

DirectedGraph::DirectedGraph(Node *nodes_, uint32_t order_) : nodes(order_) {
    for (uint32_t i = 0; i < order_; i++) {
        nodes[i] = nodes_[i];
    }
}

DirectedGraph::DirectedGraph(const std::vector<Node> &nodes_) : nodes(nodes_) {}

DirectedGraph::DirectedGraph() : nodes(0) {}

DirectedGraph::DirectedGraph(uint32_t order) : nodes(order) {}

uint32_t DirectedGraph::getNodeDegree(uint32_t u) const {
    return nodes[u].getDegree();
}

uint32_t DirectedGraph::head(uint32_t u, uint32_t k) const {
    return nodes[u].getAdj()[k].vertex;
}

uint32_t DirectedGraph::getOrder() const {
    return static_cast<uint32_t>(nodes.size());
}

void DirectedGraph::addNode(const Node &node) { nodes.emplace_back(node); }
const Node &DirectedGraph::getNode(uint32_t u) const { return nodes[u]; }
Node &DirectedGraph::getNode(uint32_t u) { return nodes[u]; }

#include <sealib/basicgraph.h>

using Sealib::BasicGraph;
using Sealib::Node;

Sealib::BasicGraph::BasicGraph(Node *nodes_, unsigned int order_) : nodes(order_) {
    for (unsigned int i = 0; i < order_; i++) {
        nodes[i] = nodes_[i];
    }
}

Sealib::BasicGraph::BasicGraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_) {}

Sealib::BasicGraph::BasicGraph() : nodes(0) {}

Sealib::BasicGraph::BasicGraph(unsigned int order) : nodes(order) {}

void Sealib::BasicGraph::addNode(const Sealib::Node &node) {
    nodes.emplace_back(node);
}
const Node &Sealib::BasicGraph::getNode(unsigned int u) const {
    return nodes[u];
}
Node &Sealib::BasicGraph::getNode(unsigned int u) {
    return nodes[u];
}
unsigned int Sealib::BasicGraph::getNodeDegree(unsigned int u) const {
    return nodes[u].getDegree();
}
unsigned int Sealib::BasicGraph::head(unsigned int u, unsigned int k) const {
    return nodes[u].getAdj()[k].vertex;
}
unsigned int Sealib::BasicGraph::getOrder() const {
    return static_cast<unsigned int>(nodes.size());
}
std::tuple<unsigned int, unsigned int>
Sealib::BasicGraph::mate(unsigned int u, unsigned int k) const {
    return std::tuple<unsigned int, unsigned int>(nodes[u].getAdj()[k].vertex,
                                                  nodes[u].getAdj()[k].crossIndex);
}

unsigned int Sealib::BasicGraph::mate2(unsigned int u, unsigned int k) const {
    return nodes[u].getAdj()[k].crossIndex;
}

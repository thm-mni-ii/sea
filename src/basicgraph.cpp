#include <sealib/basicgraph.h>

const Sealib::Node &Sealib::BasicGraph::getNode(unsigned int u) const { return nodes[u]; }

Sealib::Node &Sealib::BasicGraph::getNode(unsigned int u) { return nodes[u]; }

unsigned int Sealib::BasicGraph::getNodeDegree(unsigned int u) {
    return nodes[u].getDegree();
}

unsigned int Sealib::BasicGraph::head(unsigned int u, unsigned int k) {
    return nodes[u].getAdj()[k].vertex;
}

unsigned int Sealib::BasicGraph::getOrder() {
    return static_cast<unsigned int>(nodes.size());
}

Sealib::BasicGraph::BasicGraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_) {}

Sealib::BasicGraph::BasicGraph(Sealib::Node *nodes_, unsigned int order_) : nodes(order_) {
    for (unsigned int i = 0; i < order_; ++i) {
        nodes[i] = nodes_[i];
    }
}
void Sealib::BasicGraph::addNode(const Sealib::Node &node) {
    nodes.push_back(node);
}

Sealib::BasicGraph::BasicGraph(unsigned int order) : nodes(order) {}

Sealib::BasicGraph::BasicGraph() : BasicGraph(0) {}

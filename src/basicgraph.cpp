#include <sealib/basicgraph.h>

const Sealib::Node &Sealib::Basicgraph::getNode(unsigned int u) { return nodes[u]; }

unsigned int Sealib::Basicgraph::getNodeDegree(unsigned int u) {
    return nodes[u].getDegree();
}

unsigned int Sealib::Basicgraph::head(unsigned int u, unsigned int k) {
    return nodes[u].getAdj()[k].vertex;
}

unsigned int Sealib::Basicgraph::getOrder() {
    return static_cast<unsigned int>(nodes.size());
}

Sealib::Sealib::Basicgraph::Basicgraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_) {}

Sealib::Basicgraph::Basicgraph(Sealib::Node *nodes_, unsigned int order_) : nodes(order_) {
    for (unsigned int i = 0; i < order_; ++i) {
        nodes[i] = nodes_[i];
    }
}

#include <sealib/basicgraph.h>

const Sealib::Node &Sealib::Basicgraph::getNode(unsigned int u) const { return nodes[u]; }

unsigned int Sealib::Basicgraph::getNodeDegree(unsigned int u) const {
    return nodes[u].getDegree();
}

unsigned int Sealib::Basicgraph::head(unsigned int u, unsigned int k) const {
    return nodes[u].getAdj()[k].vertex;
}

unsigned int Sealib::Basicgraph::getOrder() const {
    return static_cast<unsigned int>(nodes.size());
}

Sealib::Basicgraph::Basicgraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_) {}

Sealib::Basicgraph::Basicgraph(Sealib::Node *nodes_, unsigned int order_) : nodes(order_) {
    for (unsigned int i = 0; i < order_; ++i) {
        nodes[i] = nodes_[i];
    }
}

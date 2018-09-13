#include "sealib/basicgraph.h"

using Sealib::Basicgraph;
using Sealib::Node;

const Node &Basicgraph::getNode(unsigned int u) { return nodes[u]; }

unsigned int Basicgraph::getNodeDegree(unsigned int u) {
    return nodes[u].getDegree();
}

unsigned int Basicgraph::head(unsigned int u, unsigned int k) {
    return nodes[u].getAdj()[k].vertex;
}

unsigned int Basicgraph::getOrder() {
    return static_cast<unsigned int>(nodes.size());
}

Sealib::Basicgraph::Basicgraph(const std::vector<Sealib::Node> &nodes_) : nodes(nodes_){

}

Basicgraph::Basicgraph(Node *nodes_, unsigned int order_) : nodes(order_) {
    for (int i = 0; i < order_; ++i) {
        nodes[i] = nodes_[i];
    }
}

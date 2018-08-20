#include "sealib/basicgraph.h"

using Sealib::Basicgraph;
using Sealib::Node;

Node *Basicgraph::getNode(unsigned int u) { return &nodes[u]; }

unsigned int Basicgraph::getNodeDegree(unsigned int  u) {
    return nodes[u].getDegree();
}

unsigned int Basicgraph::head(unsigned int u, unsigned int k) {
    return nodes[u].getAdj()[k].vertex;
}

unsigned int Basicgraph::getOrder() {
    return order;
}

Basicgraph::Basicgraph(Node *_nodes, unsigned int _order) : nodes(_nodes), order(_order) { }

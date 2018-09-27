#include "sealib/basicgraph.h"

using Sealib::BasicGraph;
using Sealib::Node;

Node *BasicGraph::getNode(unsigned int u) { return &nodes[u]; }

unsigned int BasicGraph::getNodeDegree(unsigned int  u) {
    return nodes[u].getDegree();
}

unsigned int BasicGraph::head(unsigned int u, unsigned int k) {
    return nodes[u].getAdj()[k].vertex;
}

unsigned int BasicGraph::getOrder() {
    return order;
}

BasicGraph::BasicGraph(Node *_nodes, unsigned int _order) : nodes(_nodes), order(_order) { }

BasicGraph::~BasicGraph() {
    delete nodes;
}

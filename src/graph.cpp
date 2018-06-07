#include <utility>

#include "sealib/graph.h"

const Sealib::Node &Sealib::Graph::getNode(unsigned int u) const { return nodes[u]; }

unsigned int Sealib::Graph::head(unsigned int u, unsigned int k) const { return nodes[u].getAdj(k).vertex; }

unsigned int Sealib::Graph::getOrder() const { return static_cast<unsigned int>(nodes.size()); }

void Sealib::Graph::setNodes(std::vector<Sealib::Node> nodes_) {
    nodes = std::move(nodes_);
}

Sealib::Graph::Graph() {}


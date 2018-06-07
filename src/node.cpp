#include "sealib/node.h"

std::vector<Sealib::Adjacency>& Sealib::Node::getAdj() { return adj; }

unsigned int Sealib::Node::getDegree() const { return static_cast<unsigned int>(adj.size()); }

Sealib::Node::Node()  {}

void Sealib::Node::setAdjacencies(std::vector<Sealib::Adjacency> adj_) {
    adj = adj_;
}

const Sealib::Adjacency &Sealib::Node::getAdj(unsigned int i) const {
    return adj[i];
}

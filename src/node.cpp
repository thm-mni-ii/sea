#include <sealib/node.h>

const std::vector<Sealib::Adjacency> &Sealib::Node::getAdj() { return adj; }

unsigned int Sealib::Node::getDegree() { return static_cast<unsigned int>(adj.size()); }

Sealib::Node::Node(const Sealib::Adjacency *adj_, unsigned long deg_) : adj(deg_) {
    for (unsigned long i = 0; i < deg_; i++) {
        adj[i] = adj_[i];
    }
}

Sealib::Node::Node(const std::vector<Sealib::Adjacency> &adj_) : adj(adj_) {}

void Sealib::Node::setCrossIndex(unsigned int adjIndex, unsigned int crossIndex)  {
    adj[adjIndex].crossIndex = crossIndex;
}

#include <sealib/node.h>

const std::vector<Sealib::Adjacency> &Sealib::Node::getAdj() const { return adj; }

unsigned int Sealib::Node::getDegree() const { return static_cast<unsigned int>(adj.size()); }

Sealib::Node::Node(const Sealib::Adjacency *adj_, unsigned long deg_) : adj(deg_) {
    for (unsigned long i = 0; i < deg_; i++) {
        adj[i] = adj_[i];
    }
}

Sealib::Node::Node(const std::vector<Sealib::Adjacency> &adj_) : adj(adj_) {}

void Sealib::Node::setCrossIndex(unsigned int adjIndex, unsigned int crossIndex)  {
    adj[adjIndex].crossIndex = crossIndex;
}
void Sealib::Node::addAdjacency(unsigned int vertex) {
    adj.emplace_back();
    adj[adj.size()-1].vertex = vertex;
}

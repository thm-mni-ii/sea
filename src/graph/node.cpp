#include <sealib/graph/node.h>

const std::vector<Sealib::Adjacency> &Sealib::Node::getAdj() const { return adj; }

uint32_t Sealib::Node::getDegree() const { return static_cast<uint32_t>(adj.size()); }

Sealib::Node::Node(const Sealib::Adjacency *adj_, uint64_t deg_) : adj(deg_) {
    for (uint64_t i = 0; i < deg_; i++) {
        adj[i] = adj_[i];
    }
}

Sealib::Node::Node(const std::vector<Sealib::Adjacency> &adj_) : adj(adj_) {}

void Sealib::Node::setCrossIndex(uint32_t adjIndex, uint32_t crossIndex)  {
    adj[adjIndex].crossIndex = crossIndex;
}
void Sealib::Node::addAdjacency(uint32_t vertex) {
    adj.emplace_back();
    adj[adj.size()-1].vertex = vertex;
}

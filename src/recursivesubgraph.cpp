#include <sealib/recursivesubgraph.h>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::RecursiveSubGraph::~RecursiveSubGraph() {}

Sealib::RecursiveSubGraph::RecursiveSubGraph(Sealib::SubGraph *rGraph_,
                                             Sealib::Bitset<unsigned char> &v,
                                             Sealib::Bitset<unsigned char> &e) : SubGraph(v, e) {

}
Sealib::SubGraph::bitset_t Sealib::RecursiveSubGraph::initializeQSelect() {
    return Sealib::SubGraph::bitset_t();
}
Sealib::SubGraph::bitset_t Sealib::RecursiveSubGraph::initializePSelect() {
    return Sealib::SubGraph::bitset_t();
}
unsigned long Sealib::RecursiveSubGraph::degree(unsigned long v) const {
    return 0;
}
unsigned long Sealib::RecursiveSubGraph::head(unsigned long u, unsigned long k) const {
    return 0;
}
std::tuple<unsigned long, unsigned long> Sealib::RecursiveSubGraph::mate(unsigned long u, unsigned long k) const {
    return std::tuple<unsigned long, unsigned long>();
}
unsigned long Sealib::RecursiveSubGraph::order() const {
    return 0;
}
unsigned long Sealib::RecursiveSubGraph::arcNumber(unsigned long j, unsigned long k) const {
    return 0;
}
std::tuple<unsigned long, unsigned long> Sealib::RecursiveSubGraph::inverseArcNumber(unsigned long r) const {
    return std::tuple<unsigned long, unsigned long>();
}
unsigned long Sealib::RecursiveSubGraph::translateVertex(unsigned long u) const {
    return 0;
}
unsigned long Sealib::RecursiveSubGraph::translateArc(unsigned long e) const {
    return 0;
};



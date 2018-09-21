#include <sealib/tikzgraph.h>
using SealibVisual::TikzGraph;
using std::vector;

TikzGraph::TikzGraph(unsigned int n) : nodes(n) {
    for (unsigned int i = 0; i < nodes.size(); i++) {
        nodes[i].setName(std::to_string(i));
        nodes[i].setContent(std::to_string(i));
        nodes[i].setOptions("draw, circle");
    }
}

std::ostream &TikzGraph::out(std::ostream &os) const {
    for (auto const &n : nodes) {
        os << n << std::endl;
    }
    for (auto const &v : edges) {
        os << "\\path " << v.second << ";" << std::endl;
    }
    return os;
}

bool TikzGraph::containsEdge(const key_t &key) const {
    return edges.count(key) > 0;
}

TikzGraph::edge_t &TikzGraph::addEdge(const key_t &key) {
    edges[key] = edge_t(std::get<0>(key), std::get<1>(key));
    return edges[key];
}

TikzGraph::map_t &TikzGraph::getEdges() {
    return edges;
}

vector<TikzGraph::node_t> &TikzGraph::getNodes() {
    return nodes;
}


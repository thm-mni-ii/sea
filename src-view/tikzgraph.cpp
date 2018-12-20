#include "sealibvisual/tikzgraph.h"
using SealibVisual::TikzGraph;
using SealibVisual::TikzEdge;
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
        os << n;
    }
    for (auto const &v : edges) {
        os << "\\path " << v.second << ";" << std::endl;
    }
    return os;
}

TikzEdge &TikzGraph::addEdge(const key_t &key, std::string options) {
    edges[key] = TikzEdge(std::get<0>(key), std::get<1>(key), options);
    return edges[key];
}

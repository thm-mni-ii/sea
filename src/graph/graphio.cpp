#include "sealib/graph/graphio.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "sealib/graph/directedgraph.h"
#include "sealib/graph/undirectedgraph.h"

#define LOADSTR                                               \
    if (ls->eof()) {                                          \
        if (!in.eof()) {                                      \
            std::getline(in, line);                           \
            delete ls;                                        \
            ls = new std::istringstream(line);                \
        } else {                                              \
            throw std::runtime_error("file ended too early"); \
        }                                                     \
    }                                                         \
    *ls >> (std::ws);                                         \
    std::getline(*ls, tok, ' ');

#define READ(s)                                                           \
    LOADSTR                                                               \
    if (tok != (s)) {                                                     \
        std::stringstream err;                                            \
        err << "Input file is malformed: expected '" << (s) << "', got '" \
            << tok << "'";                                                \
        throw std::runtime_error(err.str());                              \
    }
#define READL(s) \
    LOADSTR      \
    ok = tok == (s);
#define GET_CLOSING_BRACKET  \
    while (brackets > 0) {   \
        LOADSTR              \
        if (tok == "[")      \
            brackets++;      \
        else if (tok == "]") \
            brackets--;      \
    }

namespace Sealib {

void GraphExporter::exportGML(Graph const &g, bool directed,
                              const std::string& filename) {
    std::ofstream out(filename);
    out << "graph [\ndirected " << directed << "\n";
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        out << "node [\nid " << u << "\n";
        out << "]\n";
    }
    uint64_t edgeId = g.getOrder();
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        for (uint64_t k = 0; k < g.deg(u); k++) {
            out << "edge [\nid " << edgeId++ << "\n";
            out << "source " << u << "\n";
            out << "target " << g.head(u, k) << "\n";
            out << "]\n";
        }
    }
    out << "]\n";
    out.close();
}

template <class G>
static void addAdj(G *g, uint64_t u, uint64_t v);

template <>
void addAdj<DirectedGraph>(DirectedGraph *g, uint64_t u, uint64_t v) {
    g->getNode(u).addAdjacency(v);
}
template <>
void addAdj<UndirectedGraph>(UndirectedGraph *g, uint64_t u, uint64_t v) {
    uint64_t i1 = g->deg(u), i2 = g->deg(v);
    g->getNode(u).addAdjacency({v, i2});
    g->getNode(v).addAdjacency({u, i1});
}

template <class G, class N>
static G importGMLBase(const std::string& filename) {
    bool directed;
    uint trash = 0;
    uint brackets = 0;

    bool ok = true;
    std::string line;
    std::string tok;
    std::ifstream in(filename);
    std::getline(in, line);
    auto *ls = new std::istringstream(line);

    READ("graph");
    READ("[");
    do {
        READL("directed");
    } while (!ok);
    *ls >> directed;
    std::vector<N> nodes;
    while (true) {
        READL("node");
        if (ok) {
            READ("[");
            brackets++;
            READ("id");
            *ls >> trash;
            nodes.emplace_back(N());
            GET_CLOSING_BRACKET
        } else {
            ok = tok == "edge";
            break;
        }
    }
    G g(std::move(nodes));
    while (true) {
        if (ok) {
            READ("[");
            brackets++;
            READ("id");
            *ls >> trash;
            uint u, v;
            READ("source");
            *ls >> u;
            READ("target");
            *ls >> v;
            addAdj(&g, u, v);
            GET_CLOSING_BRACKET
        } else {
            ok = tok == "]";
            break;
        }
        READL("edge");
    }
    if (!ok) GET_CLOSING_BRACKET
    delete ls;
    return g;
}

template <>
UndirectedGraph GraphImporter::importGML<UndirectedGraph>(
    const std::string& filename) {
    return importGMLBase<UndirectedGraph, ExtendedNode>(filename);
}
template <>
DirectedGraph GraphImporter::importGML<DirectedGraph>(const std::string& filename) {
    return importGMLBase<DirectedGraph, SimpleNode>(filename);
}

}  // namespace Sealib

#include "sealib/graph/graphio.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "sealib/graph/directedgraph.h"
#include "sealib/graph/undirectedgraph.h"

#define READ(s)                                                           \
    if (tok[index] != s) {                                                \
        std::stringstream err;                                            \
        err << "Input file is malformed: expected '" << (s) << "', got '" \
            << tok[index] << "'";                                         \
        throw std::runtime_error(err.str());                              \
    }                                                                     \
    index++;
#define READL(s)           \
    if (tok[index] != s) { \
        ok = false;        \
    } else {               \
        ok = true;         \
        index++;           \
    }
#define GET_CLOSING_BRACKET         \
    uint64_t brackets = 1;              \
    while (brackets > 0) {          \
        if (tok[index] == "[")      \
            brackets++;             \
        else if (tok[index] == "]") \
            brackets--;             \
        index++;                    \
    }

namespace Sealib {

void GraphExporter::exportGML(Graph const *g, bool directed,
                              std::string filename) {
    std::ofstream out(filename);
    out << "graph [\ndirected " << directed << "\n";
    for (uint64_t u = 0; u < g->getOrder(); u++) {
        out << "node [\nid " << u << "\n";
        out << "]\n";
    }
    uint64_t edgeId = g->getOrder();
    for (uint64_t u = 0; u < g->getOrder(); u++) {
        for (uint64_t k = 0; k < g->deg(u); k++) {
            out << "edge [\nid " << edgeId++ << "\n";
            out << "source " << u << "\n";
            out << "target " << g->head(u, k) << "\n";
            out << "]\n";
        }
    }
    out << "]\n";
    out.close();
}

static void tokenize(const std::string &str, std::vector<std::string> *tokens,
                     const std::string &delimiters = " ",
                     bool trimEmpty = false) {
    std::string::size_type pos, lastPos = 0, length = str.length();

    while (lastPos < length + 1) {
        pos = str.find_first_of(delimiters, lastPos);
        if (pos == std::string::npos) {
            pos = length;
        }

        if (pos != lastPos || !trimEmpty)
            tokens->push_back(std::string(str.data() + lastPos, pos - lastPos));

        lastPos = pos + 1;
    }
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
static G importGMLBase(std::string filename) {
    G g(0);
    bool ok = true;

    std::ifstream in(filename);
    std::stringstream input;
    input << in.rdbuf();
    std::vector<std::string> tok;
    tokenize(input.str(), &tok, " \n", true);
    uint64_t index = 0;
    READ("graph");
    READ("[");
    do {
        READL("directed");
        index++;
    } while (!ok);
    while (true) {
        READL("node");
        if (ok) {
            READ("[");
            READ("id");
            index++;
            N u;
            g.addNode(u);
            GET_CLOSING_BRACKET
        } else {
            break;
        }
    }
    uint64_t m = 0;
    while (true) {
        READL("edge");
        if (ok) {
            READ("[");
            READ("id");
            index++;
            uint64_t u, v;
            READ("source");
            u = uint64_t(std::stoi(tok[index]));
            index++;
            READ("target");
            v = uint64_t(std::stoi(tok[index]));
            index++;
            addAdj(&g, u, v);
            GET_CLOSING_BRACKET
            m++;
        } else {
            break;
        }
    }
    READ("]");
    return g;
}

template <>
UndirectedGraph GraphImporter::importGML<UndirectedGraph>(
    std::string filename) {
    return importGMLBase<UndirectedGraph, ExtendedNode>(filename);
}
template <>
DirectedGraph GraphImporter::importGML<DirectedGraph>(std::string filename) {
    return importGMLBase<DirectedGraph, SimpleNode>(filename);
}

}  // namespace Sealib

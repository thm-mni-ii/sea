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
    uint brackets = 1;              \
    while (brackets > 0) {          \
        if (tok[index] == "[")      \
            brackets++;             \
        else if (tok[index] == "]") \
            brackets--;             \
        index++;                    \
    }

namespace Sealib {

void GraphExporter::exportGML(Graph* g, bool directed, std::string filename) {
    std::ofstream out(filename);
    out << "graph [\ndirected " << directed << "\n";
    for (uint32_t u = 0; u < g->getOrder(); u++) {
        out << "node [\nid " << u << "\n";
        out << "]\n";
    }
    uint32_t edgeId = g->getOrder();
    for (uint32_t u = 0; u < g->getOrder(); u++) {
        for (uint32_t k = 0; k < g->getNodeDegree(u); k++) {
            out << "edge [\nid " << edgeId++ << "\n";
            out << "source " << u << "\n";
            out << "target " << g->head(u, k) << "\n";
            out << "]\n";
        }
    }
    out << "]\n";
    out.close();
}

static void tokenize(const std::string& str, std::vector<std::string>* tokens,
                     const std::string& delimiters = " ",
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


template<class G>
static G importGMLBase(std::string filename) {
    G g;
    bool directed;
    bool ok = true;

    std::ifstream in(filename);
    std::stringstream input;
    input << in.rdbuf();
    std::vector<std::string> tok;
    tokenize(input.str(), &tok, " \n", true);
    uint index = 0;
    READ("graph");
    READ("[");
    do {
        READL("directed");
        index++;
    } while (!ok);
    index--;
    directed = std::stoi(tok[index]);
    index++;
    while (true) {
        READL("node");
        if (ok) {
            READ("[");
            READ("id");
            index++;
            Node u;
            g.addNode(u);
            GET_CLOSING_BRACKET
        } else {
            break;
        }
    }
    uint m = 0;
    while (true) {
        READL("edge");
        if (ok) {
            READ("[");
            READ("id");
            index++;
            uint u, v;
            READ("source");
            u = uint(std::stoi(tok[index]));
            index++;
            READ("target");
            v = uint(std::stoi(tok[index]));
            index++;
            g.getNode(u).addAdjacency(v);
            if (!directed) {
                g.getNode(v).addAdjacency(u);
                g.getNode(u).setCrossIndex(g.getNodeDegree(u) - 1,
                                            g.getNodeDegree(v) - 1);
                g.getNode(v).setCrossIndex(g.getNodeDegree(v) - 1,
                                            g.getNodeDegree(u) - 1);
            }
            GET_CLOSING_BRACKET
            m++;
        } else {
            break;
        }
    }
    READ("]");
    return g;
}

template<>
UndirectedGraph GraphImporter::importGML<UndirectedGraph>(std::string filename) {
    return importGMLBase<UndirectedGraph>(filename);
}
template<>
DirectedGraph GraphImporter::importGML<DirectedGraph>(std::string filename) {
    return importGMLBase<DirectedGraph>(filename);
}

}   // namespace Sealib

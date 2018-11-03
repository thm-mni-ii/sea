#include "sealib/graphio.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include "sealib/basicgraph.h"

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

using namespace Sealib;  // NOLINT

void GraphExporter::exportGML(Graph* g, std::string filename) {
    std::ofstream out(filename);
    out << "graph\n[\ndirected 1\n";
    for (unsigned u = 0; u < g->getOrder(); u++) {
        out << "node\n[\nid " << u << "\n";
        out << "]\n";
    }
    unsigned edgeId = g->getOrder();
    for (unsigned u = 0; u < g->getOrder(); u++) {
        for (unsigned k = 0; k < g->getNodeDegree(u); k++) {
            out << "edge\n[\nid " << edgeId++ << "\n";
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

template <>
BasicGraph GraphImporter::importGML<BasicGraph>(std::string filename) {
    bool directed;
    BasicGraph g;
    bool ok = true;
    uint n;

    std::ifstream in(filename);
    std::stringstream input;
    input << in.rdbuf();
    std::vector<std::string> tok;
    tokenize(input.str(), &tok, " \n", true);
    uint index = 0;
    READ("graph");
    READ("[");
    READ("directed");
    directed = std::stoi(tok[index]);
    std::cout << "Directed: " << directed << "\n";
    index++;
    while (true) {
        READL("node");
        if (ok) {
            READ("[");
            READ("id");
            in >> n;
            index++;
            Node u;
            g.addNode(u);
            GET_CLOSING_BRACKET
        } else {
            std::cout << "Added " << g.getOrder() << " nodes\n";
            break;
        }
    }
    uint m=0;
    while (true) {
        READL("edge");
        if (ok) {
            READ("[");
            READ("id");
            in >> n;
            index++;
            uint u, v;
            READ("source");
            in >> u;
            index++;
            READ("target");
            in >> v;
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
            std::cout << "Added " << m << " edges\n";
            break;
        }
    }
    READ("]");
    return g;
}

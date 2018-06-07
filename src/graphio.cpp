//
// Created by jmeintrup on 08.05.18.
//

#include "sealib/graphio.h"

std::string Sealib::GraphIO::stringFromTrail(const Sealib::Graph &g, Sealib::TrailStructure **trail) {
    std::string s = "\n";
    unsigned int t = 1;
    for (unsigned int i = 0; i < g.getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if (arc != (unsigned int) -1) {  // has starting arc
            s += "T" + std::to_string(t++) + ": " + std::to_string(i) + " -> ";

            unsigned int u = i;
            while (arc != (unsigned int) - 1) {
                unsigned int uCross = g.getNode(u).getAdj(arc).crossIndex;
                u = g.getNode(u).getAdj(arc).vertex;

                s += std::to_string(u);
                // arc = trail[u]->getMatched(uCross);
                arc = trail[u]->getMatched(uCross);
                if (arc == uCross) {
                    arc = (unsigned int) -1;
                    s+="\n";
                } else {
                    s+=" -> ";
                }
            }
        }
    }
    return s;
}

void Sealib::GraphIO::dotFileFromTrail(const Sealib::Graph &g, Sealib::TrailStructure **trail, std::string fileName) {
    const std::string colors[10] = {
            "red",
            "blue",
            "green",
            "violet",
            "yellow",
            "indigo",
            "cyan",
            "teal",
            "navy",
            "gold"
    };

    std::ofstream o(fileName);
    o << "Digraph G {";
    unsigned int t = 0;
    for (unsigned int i = 0; i < g.getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if (arc != (unsigned int) -1) {  // has starting arc
            o << "\n\t" << std::to_string(i) << " -> ";
            unsigned int u = i;
            unsigned int cnt = 1;
            while (arc != (unsigned int) - 1) {
                unsigned int uCross = g.getNode(u).getAdj(arc).crossIndex;
                u = g.getNode(u).getAdj(arc).vertex;

                o << std::to_string(u) << "[label=" << std::to_string(cnt++) << ", color=" << colors[t % 10] << "]";

                arc = trail[u]->getMatched(uCross);
                if (arc == uCross) {
                    arc = (unsigned int) -1;
                } else {
                    o << "\n\t" << std::to_string(u) << " -> ";
                }
            }
            t++;
        }
    }
    o << "\n}";
}

void Sealib::GraphIO::dotFileFromGraph(const Sealib::Graph &g, std::string fileName) {
    std::vector<std::vector<bool>> edgeTaken = std::vector<std::vector<bool>>();
    for (unsigned int i = 0; i < g.getOrder();  i++) {
        unsigned int degree = g.getNode(i).getDegree();

        edgeTaken.push_back(std::vector<bool>());

        for (unsigned int j = 0; j < degree; j++) {
            edgeTaken.at(i).push_back(0);
        }
    }

    std::ofstream o(fileName);
    o << "Graph G {";
    for (unsigned int i = 0; i < g.getOrder();  i++) {
        const Sealib::Node &n = g.getNode(i);
        unsigned int degree = g.getNode(i).getDegree();
        for (unsigned int j = 0; j < degree; j++) {
            if (edgeTaken.at(i).at(j) == 0) {
                unsigned int vertex = n.getAdj(j).vertex;
                unsigned int crossIndex = n.getAdj(j).crossIndex;

                o << "\n\t" << std::to_string(i) << " -- " << vertex;
                edgeTaken.at(i).at(j) = 1;
                edgeTaken.at(vertex).at(crossIndex) = 1;
            }
        }
    }
    o << "\n}";
}

void Sealib::GraphIO::randomEulerianSealibGraph(int nodeMax, int edgeMax, Sealib::Graph &g) {
    ogdf::Graph ogdfGraph;
    randomEulerianOgdfGraph(nodeMax, edgeMax, ogdfGraph);
    sealibGraphFromOgdfGraph(ogdfGraph, g);
}

void Sealib::GraphIO::randomEulerianOgdfGraph(int nodeMax, int edgeMax, ogdf::Graph &g) {
    ogdf::List<std::pair<int, int>> edges;

    std::vector<int> uneven = std::vector<int>();
    std::vector<int> even = std::vector<int>();

    uneven.push_back(0);
    uneven.push_back(nodeMax - 1);

    for (int i = 1; i < nodeMax - 1; i++) {
        even.push_back(i);
    }

    for (int i = 0; i < nodeMax - 1; i++) {
        edges.pushBack(std::pair<int, int>(i, i+1));
    }

    while (edges.size() < edgeMax) {
        int nextEven;
        int r1 = static_cast<int>(rand() % (even.size() - 1));

        nextEven = even.at(r1);

        int nextUneven;
        int r2 = static_cast<int>(rand() % (uneven.size() - 1));

        nextUneven = uneven.at(r2);

        edges.pushBack(std::pair<int, int>(nextEven, nextUneven));

        even.at(r1) = nextUneven;
        uneven.at(r2) = nextEven;
    }

    ogdf::customGraph(g, nodeMax, edges);
}

void Sealib::GraphIO::ogdfGraphFromSealibGraph(const Sealib::Graph &sealibGraph, ogdf::Graph &ogdfGraph) {
    std::vector<std::vector<bool>> edgeTaken = std::vector<std::vector<bool>>();

    for (unsigned int i = 0; i < sealibGraph.getOrder();  i++) {
        unsigned int degree = sealibGraph.getNode(i).getDegree();

        edgeTaken.push_back(std::vector<bool>());

        for (unsigned int j = 0; j < degree; j++) {
            edgeTaken.at(i).push_back(0);
        }

        ogdfGraph.newNode(i);
    }

    ogdf::List<ogdf::node> nodes;
    ogdfGraph.allNodes(nodes);
    for (auto ogdfFrom : nodes) {
        unsigned int i = static_cast<unsigned int>(ogdfFrom->index());
        const Sealib::Node &sealibNode = sealibGraph.getNode(i);
        unsigned int degree = sealibGraph.getNode(i).getDegree();

        for (unsigned int j = 0; j < degree; j++) {
            if (edgeTaken.at(i).at(j) == 0) {
                unsigned int vertex = sealibNode.getAdj(j).vertex;
                unsigned int crossIndex = sealibNode.getAdj(j).crossIndex;

                edgeTaken.at(i).at(j) = true;
                edgeTaken.at(vertex).at(crossIndex) = true;
                for (auto ogdfTo : nodes) {
                    if ((unsigned int) ogdfTo->index() == vertex) {
                        ogdfGraph.newEdge(ogdfFrom, ogdfTo);
                        break;
                    }
                }
            }
        }
    }
}

void Sealib::GraphIO::sealibGraphFromOgdfGraph(const ogdf::Graph &ogdfGraph, Sealib::Graph &sealibGraph) {
    unsigned int order = static_cast<unsigned int>(ogdfGraph.nodes.size());

    std::vector<Sealib::Node> sealibNodesVector(order);
    std::vector<ogdf::node> ogdfNodesVector;
    ogdfNodesVector.reserve(order);

    ogdf::List<ogdf::node> ogdfNodes;
    ogdfGraph.allNodes(ogdfNodes);

    for (auto n : ogdfNodes) {
        ogdfNodesVector.push_back(n);
    }

    for (unsigned int i = 0; i < order; i++) {
        auto deg = (unsigned int) ogdfNodesVector[i]->degree();

        std::vector<Sealib::Adjacency> adj(deg);
        ogdf::List<ogdf::edge> edges;
        ogdfNodesVector[i]->adjEdges(edges);

        unsigned int idx = 0;
        for (auto e : edges) {
            if ((unsigned int) e->target()->index() == i) {  // source is the one to add to the adjacency
                adj[idx++] = Adjacency(static_cast<unsigned int>(e->source()->index()));
            } else {
                adj[idx++] = Adjacency(static_cast<unsigned int>(e->target()->index()));  // target is the one to add
            }
        }
        sealibNodesVector[i].setAdjacencies(adj);
    }

    for (unsigned int i = 0; i < order; i++) {
        const unsigned int deg = sealibNodesVector[i].getDegree();
        std::vector<Sealib::Adjacency> &adj_arr = sealibNodesVector[i].getAdj();

        for (unsigned int j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                std::vector<Sealib::Adjacency> &_adj_arr = sealibNodesVector[v].getAdj();
                const unsigned int _deg = sealibNodesVector[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex == std::numeric_limits<unsigned int>::max() && _adj_arr[_j].vertex == i) {
                        _adj_arr[_j].crossIndex = j;
                        adj_arr[j].crossIndex = _j;
                        break;
                    }
                }
            }
        }
    }
    sealibGraph.setNodes(sealibNodesVector);
}

void Sealib::GraphIO::graphAttributesFromTrail(ogdf::GraphAttributes &GA, const Sealib::Graph &g, Sealib::TrailStructure **trail) {
    const std::string colors[10] = {
            "#ff0000",
            "#003366",
            "#008000",
            "#EE82EE",
            "#FFFF00",
            "#4B0082",
            "#00FFFF",
            "#008080",
            "#000080",
            "#FFD700"
    };

    ogdf::List<ogdf::edge> edges;
    GA.constGraph().allEdges(edges);

    unsigned int t = 0;
    for (unsigned int i = 0; i < g.getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if (arc != (unsigned int) -1) {  // has starting arc
            unsigned int u = i;
            unsigned int cnt = 1;
            while (arc != (unsigned int) - 1) {
                unsigned int uCross = g.getNode(u).getAdj(arc).crossIndex;

                unsigned int v = u;
                u =  g.getNode(u).getAdj(arc).vertex;

                arc = trail[u]->getMatched(uCross);
                if (arc == uCross) {
                    arc = (unsigned int) -1;
                }
                std::map<ogdf::edge, int> multiEdgeMap;

                for (auto e : edges) {
                    if ((unsigned int) e->source()->index() == v && (unsigned int) e->target()->index() == u) {  // start at source, go to target
                        GA.arrowType(e) = ogdf::EdgeArrow::Last;
                        GA.strokeColor(e) = ogdf::Color(colors[t % 10]);
                        GA.strokeWidth(e) = 1;
                        GA.label(e) = std::to_string(cnt++);
                        edges.removeFirst(e);
                        break;
                    } else if ((unsigned int) e->source()->index() == u && (unsigned int) e->target()->index() == v) {  // start at target, go to source
                        GA.arrowType(e) = ogdf::EdgeArrow::First;
                        GA.strokeColor(e) = ogdf::Color(colors[t % 10]);
                        GA.bends(e);
                        GA.strokeWidth(e) = 1;
                        GA.label(e) = std::to_string(cnt++);
                        edges.removeFirst(e);
                        break;
                    }
                }
            }
            t++;
        }
    }
}


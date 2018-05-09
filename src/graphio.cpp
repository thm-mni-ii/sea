//
// Created by jmeintrup on 08.05.18.
//

#include "sealib/graphio.h"

std::string Sealib::GraphIO::stringFromTrail(Sealib::Graph *g, Sealib::TrailStructure **trail) {
    std::string s = "\n";
    unsigned int t = 1;
    for (unsigned int i = 0; i < g->getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if (arc != (unsigned int) -1) {  // has starting arc
            s += "T" + std::to_string(t++) + ": " + std::to_string(i) + " -> ";

            unsigned int u = i;
            while (arc != (unsigned int) - 1) {
                unsigned int uCross = g->getNode(u)->getAdj()[arc].crossIndex;
                u = g->getNode(u)->getAdj()[arc].vertex;

                s += std::to_string(u);
                // arc = trail[u]->getMatched(uCross);
                arc = trail[u]->getMatchedNew(uCross);
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

void Sealib::GraphIO::dotFileFromTrail(Sealib::Graph *g, Sealib::TrailStructure **trail, std::string fileName) {
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
    for (unsigned int i = 0; i < g->getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if (arc != (unsigned int) -1) {  // has starting arc
            o << "\n\t" << std::to_string(i) << " -> ";
            unsigned int u = i;
            unsigned int cnt = 1;
            while (arc != (unsigned int) - 1) {
                unsigned int uCross = g->getNode(u)->getAdj()[arc].crossIndex;
                u = g->getNode(u)->getAdj()[arc].vertex;

                o << std::to_string(u) << "[label=" << std::to_string(cnt++) << ", color=" << colors[t % 10] << "]";

                arc = trail[u]->getMatchedNew(uCross);
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

void Sealib::GraphIO::dotFileFromGraph(Sealib::Graph *g, std::string fileName) {
    std::vector<std::vector<bool>> edgeTaken = std::vector<std::vector<bool>>();
    for (unsigned int i = 0; i < g->getOrder();  i++) {
        unsigned int degree = g->getNode(i)->getDegree();

        edgeTaken.push_back(std::vector<bool>());

        for (unsigned int j = 0; j < degree; j++) {
            edgeTaken.at(i).push_back(0);
        }
    }

    std::ofstream o(fileName);
    o << "Graph G {";
    for (unsigned int i = 0; i < g->getOrder();  i++) {
        Sealib::Node *n = g->getNode(i);
        unsigned int degree = g->getNode(i)->getDegree();
        for (unsigned int j = 0; j < degree; j++) {
            if (edgeTaken.at(i).at(j) == 0) {
                unsigned int vertex = n->getAdj()[j].vertex;
                unsigned int crossIndex = n->getAdj()[j].crossIndex;

                o << "\n\t" << std::to_string(i) << " -- " << vertex;
                edgeTaken.at(i).at(j) = 1;
                edgeTaken.at(vertex).at(crossIndex) = 1;
            }
        }
    }
    o << "\n}";
}

Sealib::Graph* Sealib::GraphIO::randomEulerianSealibGraph(int nodeMax, int edgeMax) {
    ogdf::Graph *g = randomEulerianOgdfGrah(nodeMax, edgeMax);
    return sealibGraphFromOgdfGraph(g);
}

ogdf::Graph* Sealib::GraphIO::randomEulerianOgdfGrah(int nodeMax, int edgeMax) {
    ogdf::Graph *g = new ogdf::Graph();
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

    ogdf::customGraph(*g, nodeMax, edges);

    return g;
}

ogdf::Graph* Sealib::GraphIO::ogdfGraphFromSealibGraph(Sealib::Graph *g) {
    auto *G = new ogdf::Graph();
    std::vector<std::vector<bool>> edgeTaken = std::vector<std::vector<bool>>();

    for (unsigned int i = 0; i < g->getOrder();  i++) {
        unsigned int degree = g->getNode(i)->getDegree();

        edgeTaken.push_back(std::vector<bool>());

        for (unsigned int j = 0; j < degree; j++) {
            edgeTaken.at(i).push_back(0);
        }

        G->newNode(i);
    }

    ogdf::List<ogdf::node> nodes;
    G->allNodes(nodes);
    for (auto ogdfFrom : nodes) {
        unsigned int i = ogdfFrom->index();
        Sealib::Node *sealibNode = g->getNode(i);
        unsigned int degree = g->getNode(i)->getDegree();

        for (unsigned int j = 0; j < degree; j++) {
            if (edgeTaken.at(i).at(j) == 0) {
                unsigned int vertex = sealibNode->getAdj()[j].vertex;
                unsigned int crossIndex = sealibNode->getAdj()[j].crossIndex;

                edgeTaken.at(i).at(j) = 1;
                edgeTaken.at(vertex).at(crossIndex) = 1;
                for (auto ogdfTo : nodes) {
                    if ((unsigned int) ogdfTo->index() == vertex) {
                        G->newEdge(ogdfFrom, ogdfTo);
                        break;
                    }
                }
            }
        }
    }
    return G;
}

Sealib::Graph* Sealib::GraphIO::sealibGraphFromOgdfGraph(ogdf::Graph *g) {
    auto order = (unsigned int) g->nodes.size();

    auto *sealibNodesArray = static_cast<Node *>(malloc(sizeof(Sealib::Node) * order));
    auto *ogdfNodesArray = static_cast<ogdf::node *>(malloc(sizeof(ogdf::node) * order));

    ogdf::List<ogdf::node> ogdfNodes;
    g->allNodes(ogdfNodes);

    unsigned int k = 0;
    for (auto n : ogdfNodes) {
        ogdfNodesArray[k++] = n;
    }

    for (unsigned int i = 0; i < order; i++) {
        auto deg = (unsigned int) ogdfNodesArray[i]->degree();

        auto *adj = (Sealib::Adjacency *) malloc(sizeof(Sealib::Adjacency*) * deg);

        ogdf::List<ogdf::edge> edges;
        ogdfNodesArray[i]->adjEdges(edges);

        unsigned int idx = 0;
        for (auto e : edges) {
            if ((unsigned int) e->target()->index() == i) {  // source is the one to add to the adjacency
                adj[idx++] = Adjacency(e->source()->index());
            } else {
                adj[idx++] = Adjacency(e->target()->index());  // target is the one to add
            }
        }
        sealibNodesArray[i] = Sealib::Node(adj, deg);
    }

    free(ogdfNodesArray);

    for (unsigned int i = 0; i < order; i++) {
        const unsigned int deg = sealibNodesArray[i].getDegree();
        Sealib::Adjacency *adj_arr = sealibNodesArray[i].getAdj();

        for (unsigned int j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                Sealib::Adjacency *_adj_arr = sealibNodesArray[v].getAdj();
                const unsigned int _deg = sealibNodesArray[v].getDegree();

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

    return new Sealib::Graph(sealibNodesArray, order);
}

void Sealib::GraphIO::graphAttributesFromTrail(ogdf::GraphAttributes *GA, Sealib::Graph *g, Sealib::TrailStructure **trail) {
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
    GA->constGraph().allEdges(edges);

    unsigned int t = 0;
    for (unsigned int i = 0; i < g->getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if (arc != (unsigned int) -1) {  // has starting arc
            unsigned int u = i;
            unsigned int cnt = 1;
            while (arc != (unsigned int) - 1) {
                unsigned int uCross = g->getNode(u)->getAdj()[arc].crossIndex;

                unsigned int v = u;
                u = g->getNode(u)->getAdj()[arc].vertex;

                arc = trail[u]->getMatchedNew(uCross);
                if (arc == uCross) {
                    arc = (unsigned int) -1;
                }
                std::map<ogdf::edge, int> multiEdgeMap;

                for (auto e : edges) {
                    if ((unsigned int) e->source()->index() == v && (unsigned int) e->target()->index() == u) {  // start at source, go to target
                        GA->arrowType(e) = ogdf::EdgeArrow::Last;
                        GA->strokeColor(e) = ogdf::Color(colors[t % 10]);
                        GA->strokeWidth(e) = 1;
                        GA->label(e) = std::to_string(cnt++);
                        edges.removeFirst(e);
                        break;
                    } else if ((unsigned int) e->source()->index() == u && (unsigned int) e->target()->index() == v) {  // start at target, go to source
                        GA->arrowType(e) = ogdf::EdgeArrow::First;
                        GA->strokeColor(e) = ogdf::Color(colors[t % 10]);
                        GA->bends(e);
                        GA->strokeWidth(e) = 1;
                        GA->label(e) = std::to_string(cnt++);
                        edges.removeFirst(e);
                        break;
                    }
                }
            }
            t++;
        }
    }
}

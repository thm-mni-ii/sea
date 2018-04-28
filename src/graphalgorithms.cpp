//
// Created by jmeintrup on 13.04.18.
//


#include <cstdlib>
#include <iostream>
#include <fstream>
#include <list>
#include <ogdf/basic/graphics.h>
#include <ogdf/basic/GraphAttributes.h>
#include <thread>
#include <zconf.h>
#include <set>
#include <ogdf/basic/graph_generators.h>
#include "sealib/graphalgorithms.h"

using namespace std;

TrailStructure ** GraphAlgorithms::hierholzer(Sealib::Graph *g) {

    unsigned  int order = g->getOrder();
    TrailStructure **ts = static_cast<TrailStructure **>(malloc((sizeof(TrailStructure *) * order)));

    for(unsigned int i = 0; i < order; i ++) {
        unsigned  int degree = g->getNode(i)->getDegree();
        ts[i] = new TrailStructure(degree, i);
    }

    //find first start node
    unsigned int u = (unsigned int) - 1;
    for(unsigned int i = 0; i < order; i ++) {
        if(g->getNode(i)->getDegree() % 2 != 0) { //odd
            u = i;
            break;
        }
    }
    if(u == (unsigned int) - 1) { //no odd found
        for(unsigned int i = 0; i < order; i ++) {
            if(g->getNode(i)->getDegree() != 0) { //first that has edges, it's possible to have a graph with no edges
                u = i;
                break;
            }
        }
    }

    while(u != (unsigned int)  -1) { //loop the iteration while there is a non-black vertex
        unsigned int kOld = (unsigned int) - 1;
        if(ts[u]->isEven() && ts[u]->isGrey()) { //remember aOld
            kOld = ts[u]->getLastClosed();
        }
        unsigned int kFirst = ts[u]->leave();
        //unsigned int kFirstMate = g.getNode(u)->getAdj()[kFirst].crossIndex;

        unsigned int k = kFirst;
        unsigned int uMate;
        do {
            uMate = g->getNode(u)->getAdj()[k].crossIndex;
            u = g->getNode(u)->getAdj()[k].vertex; //next node
            k = ts[u]->enter(uMate);
        } while(k != (unsigned int) - 1);



        if(kOld != (unsigned int) - 1) {
            unsigned int kLast = uMate;

            unsigned int kOldMatch = ts[u]->getMatched(kOld);
            if(kOldMatch != kOld) { //has match
                ts[u]->marry(kOldMatch, kFirst);
                ts[u]->marry(kLast, kOld);
            } else {
                ts[u]->marry(kLast, kOld);
            }
        }

        //find next start node
        u = (unsigned int) - 1;

        for(unsigned int i = 0; i < order; i ++) {
            if(!ts[i]->isEven() && !ts[i]->isBlack()) { //odd
                u = i;
                break;
            }
        }
        if(u == (unsigned int) - 1) { //no odd found, search for grey
            for(unsigned int i = 0; i < order; i ++) {
                if(ts[i]->isGrey() && !ts[i]->isBlack()) { //first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        if(u == (unsigned int) - 1) { //no odd found and no grey found, search for white
            for(unsigned int i = 0; i < order; i ++) {
                if(!ts[i]->isBlack()) { //first that has edges, it's possible to have a graph with no edges
                    u = i;
                    break;
                }
            }
        }
        //no node left, should break now.
    }

    return ts;
}

std::string GraphAlgorithms::stringFromTrail(Sealib::Graph *g, TrailStructure **trail) {
    std::string s = "\n";
    unsigned int t = 1;
    for(unsigned int i = 0; i < g->getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if(arc != (unsigned int) -1) { //has starting arc
            s += "T" + std::to_string(t++) + ": " + std::to_string(i) + " -> ";

            unsigned int u = i;
            while(arc != (unsigned int) - 1) {
                unsigned int uCross = g->getNode(u)->getAdj()[arc].crossIndex;
                u = g->getNode(u)->getAdj()[arc].vertex;

                s += std::to_string(u);
                arc = trail[u]->getMatched(uCross);
                if(arc == uCross) {
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

void GraphAlgorithms::dotFileFromTrail(Sealib::Graph *g, TrailStructure **trail, std::string fileName) {
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
    for(unsigned int i = 0; i < g->getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if(arc != (unsigned int) -1) { //has starting arc
            o << "\n\t" << std::to_string(i) << " -> ";
            unsigned int u = i;
            unsigned int cnt = 1;
            while(arc != (unsigned int) - 1) {
                unsigned int uCross = g->getNode(u)->getAdj()[arc].crossIndex;
                u = g->getNode(u)->getAdj()[arc].vertex;

                o << std::to_string(u) << "[label=" << std::to_string(cnt++) << ", color=" << colors[t % 10] << "]";

                arc = trail[u]->getMatched(uCross);
                if(arc == uCross) {
                    arc = (unsigned int) -1;
                } else {
                    o << "\n\t" << std::to_string(u) << " -> " ;
                }
            }
            t++;
        }
    }
    o << "\n}";
}


void GraphAlgorithms::dotFileFromGraph(Sealib::Graph *g, std::string fileName) {
    vector<vector<bool>> edgeTaken = vector<vector<bool>>();
    for(unsigned int i = 0; i < g->getOrder();  i++) {
        unsigned int degree = g->getNode(i)->getDegree();

        edgeTaken.push_back(vector<bool>());

        for(unsigned int j = 0; j < degree; j++) {
            edgeTaken.at(i).push_back(0);
        }
    }

    std::ofstream o(fileName);
    o << "Graph G {";
    for(unsigned int i = 0; i < g->getOrder();  i++) {
        Node *n = g->getNode(i);
        unsigned int degree = g->getNode(i)->getDegree();
        for(unsigned int j = 0; j < degree; j++) {
            if(edgeTaken.at(i).at(j) == 0) {
                unsigned int vertex = n->getAdj()[j].vertex;
                unsigned int crossIndex = n->getAdj()[j].crossIndex;

                o << "\n\t" << to_string(i) << " -- " << vertex;
                edgeTaken.at(i).at(j) = 1;
                edgeTaken.at(vertex).at(crossIndex) = 1;
            }
        }
    }
    o << "\n}";
}

Sealib::Graph* GraphAlgorithms::randomEulerianSealibGraph(int nodeMax, int edgeMax) {
    ogdf::Graph *g = randomEulerianOgdfGrah(nodeMax, edgeMax);
    return sealibGraphFromOgdfGraph(g);
}

ogdf::Graph* GraphAlgorithms::randomEulerianOgdfGrah(int nodeMax, int edgeMax) {
    ogdf::Graph *g = new ogdf::Graph();
    ogdf::List<std::pair<int,int>> edges;

    std::set<int> uneven = set<int>();
    uneven.insert(0);
    uneven.insert(nodeMax-1);

    std::set<int> even = set<int>();

    for(int i = 1; i < nodeMax - 1; i++) {
        even.insert(i);
    }

    for(int i = 0; i < nodeMax - 1; i++) {
        edges.pushBack(pair<int,int>(i,i+1));
    }

    while(edges.size() < edgeMax) {
        if(uneven.empty() || even.empty()) {
            break;
        }

        int nextEven;
        int r = (int) (rand() % even.size());
        auto it = even.begin();
        for (; r != 0; r--) it++;
        nextEven = *it;

        int nextUneven;
        r = (int) (rand() % uneven.size());
        it = uneven.begin();
        for (; r != 0; r--) it++;
        nextUneven = *it;

        even.erase(nextEven);
        uneven.erase(nextUneven);

        even.insert(nextUneven);
        uneven.insert(nextEven);

        edges.pushBack(pair<int,int>(nextEven, nextUneven));
    }

    ogdf::customGraph(*g, nodeMax, edges);

    return g;
}

ogdf::Graph* GraphAlgorithms::ogdfGraphFromSealibGraph(Sealib::Graph *g) {

    ogdf::Graph *G = new ogdf::Graph();
    vector<vector<bool>> edgeTaken = vector<vector<bool>>();

    for(unsigned int i = 0; i < g->getOrder();  i++) {
        unsigned int degree = g->getNode(i)->getDegree();

        edgeTaken.push_back(vector<bool>());

        for(unsigned int j = 0; j < degree; j++) {
            edgeTaken.at(i).push_back(0);
        }

        G->newNode(i);
    }

    ogdf::List<ogdf::node> nodes;
    G->allNodes(nodes);
    for(auto ogdfFrom: nodes) {
        unsigned int i = ogdfFrom->index();
        Node *sealibNode = g->getNode(i);
        unsigned int degree = g->getNode(i)->getDegree();

        for(unsigned int j = 0; j < degree; j++) {
            if(edgeTaken.at(i).at(j) == 0) {
                unsigned int vertex = sealibNode->getAdj()[j].vertex;
                unsigned int crossIndex = sealibNode->getAdj()[j].crossIndex;

                edgeTaken.at(i).at(j) = 1;
                edgeTaken.at(vertex).at(crossIndex) = 1;
                for(auto ogdfTo: nodes) {
                    if((unsigned int) ogdfTo->index() == vertex) {
                        G->newEdge(ogdfFrom, ogdfTo);
                        break;
                    }
                }
            }
        }
    }
    return G;
}

Sealib::Graph* GraphAlgorithms::sealibGraphFromOgdfGraph(ogdf::Graph *g) {
    unsigned int order = (unsigned int) g->nodes.size();

    auto *sealibNodesArray = (Node *) malloc(sizeof(Node) * order);
    auto *ogdfNodesArray = (ogdf::node *) malloc(sizeof(ogdf::node) * order);

    ogdf::List<ogdf::node> ogdfNodes;
    g->allNodes(ogdfNodes);

    unsigned int k = 0;
    for(auto n: ogdfNodes) {
        ogdfNodesArray[k++] = n;
    }

    for(unsigned int i = 0; i < order; i++) {
        auto deg = (unsigned int) ogdfNodesArray[i]->degree();

        auto *adj = (Adjacency *) malloc(sizeof(Adjacency*) * deg);

        ogdf::List<ogdf::edge> edges;
        ogdfNodesArray[i]->adjEdges(edges);

        unsigned int idx = 0;
        for(auto e: edges) {
            if((unsigned int) e->target()->index() == i) { //source is the one to add to the adjacency
                adj[idx++] = e->source()->index();
            } else {
                adj[idx++] = e->target()->index(); //target is the one to add
            }
        }
        sealibNodesArray[i] = Node(adj, deg);
    }

    free(ogdfNodesArray);

    for (unsigned int i = 0; i < order; i++) {
        const unsigned int deg = sealibNodesArray[i].getDegree();
        Adjacency *adj_arr = sealibNodesArray[i].getAdj();

        for (unsigned int j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {

                unsigned int v = adj_arr[j].vertex;
                Adjacency *_adj_arr = sealibNodesArray[v].getAdj();
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

void GraphAlgorithms::graphAttributesFromTrail(ogdf::GraphAttributes *GA, Sealib::Graph *g, TrailStructure **trail) {

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

    /*ogdf::List<ogdf::node> nodes;
    GA->constGraph().allNodes(nodes);
    for(auto v: nodes) {
        GA->fillColor(v) = ogdf::Color( "#FFFF00" ); // set node color to yellow

        GA->height(v) = 20.0; // set the height to 20.0
        GA->width(v) = 20.0; // set the width to 40.0
        GA->shape(v) = ogdf::Shape::Ellipse;

        string s = to_string(v->index());
        char const *pchar = s.c_str(); //use char const* as target type
        GA->label(v) = pchar;
    }*/

    ogdf::List<ogdf::edge> edges;
    GA->constGraph().allEdges(edges);

    unsigned int t = 0;
    for(unsigned int i = 0; i < g->getOrder(); i++) {
        unsigned int arc = trail[i]->getStartingArc();
        if(arc != (unsigned int) -1) { //has starting arc

            unsigned int u = i;
            unsigned int cnt = 1;
            while(arc != (unsigned int) - 1) {
                unsigned int uCross = g->getNode(u)->getAdj()[arc].crossIndex;

                unsigned int v = u;
                u = g->getNode(u)->getAdj()[arc].vertex;

                arc = trail[u]->getMatched(uCross);
                if(arc == uCross) {
                    arc = (unsigned int) -1;
                }
                map<ogdf::edge, int> multiEdgeMap;

                for(auto e: edges) {
                    if((unsigned int) e->source()->index() == v && (unsigned int) e->target()->index() == u) { //start at source, go to target
                        GA->arrowType(e) = ogdf::EdgeArrow::Last;
                        GA->strokeColor(e) = ogdf::Color(colors[t % 10]);
                        GA->strokeWidth(e) = 1;
                        GA->label(e) = std::to_string(cnt++);
                        edges.removeFirst(e);
                        break;
                    } else if((unsigned int) e->source()->index() == u && (unsigned int) e->target()->index() == v) { //start at target, go to source
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

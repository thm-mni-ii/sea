//
// Created by jmeintrup on 13.04.18.
//


#include <cstdlib>
#include <iostream>
#include <fstream>
#include "sealib/graphalgorithms.h"

using namespace std;

TrailStructure ** GraphAlgorithms::hierholzer(Graph *g) {

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

std::string GraphAlgorithms::stringFromTrail(Graph *g, TrailStructure **trail) {
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

void GraphAlgorithms::dotFileFromTrail(Graph *g, TrailStructure **trail, std::string fileName) {
    const std::string colors[10] = {
            "red",
            "blue",
            "green",
            "violet",
            "yellow",
            "indogo",
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

void GraphAlgorithms::dotFileFromGraph(Graph *g, std::string fileName) {
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


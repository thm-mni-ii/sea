//
// Created by jmeintrup on 13.04.18.
//

#ifndef SEA_GRAPHALGORITHMS_H
#define SEA_GRAPHALGORITHMS_H


#include "graph.h"
#include "trailstructure.h"
#include <string>
#include <sstream>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <src/graphView.h>

class GraphAlgorithms {

public:
    static TrailStructure ** hierholzer(Sealib::Graph *g);

    static std::string stringFromTrail(Sealib::Graph *g, TrailStructure **trail);

    static void dotFileFromTrail(Sealib::Graph *g, TrailStructure **trail, std::string fileName);

    static void dotFileFromGraph(Sealib::Graph *g, std::string fileName);

    static ogdf::Graph* ogdfGraphFromSealibGraph(Sealib::Graph *g);

    static void graphAttributesFromTrail(ogdf::GraphAttributes*GA, Sealib::Graph *g, TrailStructure **trail);

    static Sealib::Graph* sealibGraphFromOgdfGraph(ogdf::Graph *g);

    static ogdf::Graph *randomEulerianOgdfGrah(int nodeMax, int edgeMax);

    Sealib::Graph *randomEulerianSealibGraph(int nodeMax, int edgeMax);
};


#endif //SEA_GRAPHALGORITHMS_H

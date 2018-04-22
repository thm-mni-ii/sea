//
// Created by jmeintrup on 13.04.18.
//

#ifndef SEA_GRAPHALGORITHMS_H
#define SEA_GRAPHALGORITHMS_H


#include "graph.h"
#include "trailstructure.h"
#include <string>
#include <sstream>

class GraphAlgorithms {

public:
    static TrailStructure ** hierholzer(Graph *g);

    static std::string stringFromTrail(Graph *g, TrailStructure **trail);

    static void dotFileFromTrail(Graph *g, TrailStructure **trail, std::string fileName);

    static void dotFileFromGraph(Graph *g, std::string fileName);
};


#endif //SEA_GRAPHALGORITHMS_H

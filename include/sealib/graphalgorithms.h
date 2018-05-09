//
// Created by jmeintrup on 13.04.18.
//

#ifndef SEA_GRAPHALGORITHMS_H
#define SEA_GRAPHALGORITHMS_H

#include "sealib/graph.h"
#include "sealib/trailstructure.h"

/**
 * Collection of Space Efficient Algorithms for Graphs.
 * @author Johannes Meintrup
 */
class GraphAlgorithms {
 public:
    /**
     * Space efficient implementation of the hierholzer algorithm
     * to find euler trails in a given graph.
     * @param g Graph to be traversed
     * @return Array of Sealib::TrailStrucutre Objects representing the trail
     */
    static Sealib::TrailStructure ** hierholzer(Sealib::Graph *g);
};

#endif  // SEA_GRAPHALGORITHMS_H
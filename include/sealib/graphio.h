//
// Created by jmeintrup on 08.05.18.
//

#ifndef SEA_GRAPHIO_H
#define SEA_GRAPHIO_H
#include <string>
#include "graph.h"
#include "trailstructure.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>

namespace Sealib {
    class GraphIO {
    public:
        static std::string stringFromTrail(Sealib::Graph *g, Sealib::TrailStructure **trail);

        static void dotFileFromTrail(Sealib::Graph *g, Sealib::TrailStructure **trail, std::string fileName);

        static void dotFileFromGraph(Sealib::Graph *g, std::string fileName);

        static ogdf::Graph* ogdfGraphFromSealibGraph(Sealib::Graph *g);

        static void graphAttributesFromTrail(ogdf::GraphAttributes*GA, Sealib::Graph *g, Sealib::TrailStructure **trail);

        static Sealib::Graph* sealibGraphFromOgdfGraph(ogdf::Graph *g);

        static ogdf::Graph *randomEulerianOgdfGrah(int nodeMax, int edgeMax);

        static Sealib::Graph *randomEulerianSealibGraph(int nodeMax, int edgeMax);
    };
}
#endif //SEA_GRAPHIO_H

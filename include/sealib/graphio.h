//
// Created by jmeintrup on 08.05.18.
//

#ifndef SEA_GRAPHIO_H
#define SEA_GRAPHIO_H

#include <string>
#include "sealib/graph.h"
#include "sealib/trailstructure.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/graph_generators.h>

namespace Sealib {
/**
* Collection of static functions for input/output of graphs,
* conversion of Sealib::Graph to ogdf::Graph,
* output to different filetypes.
* @author Johannes Meintrup
*/
class GraphIO {
 public:
    /**
     * Retunrs the euler trails represented by trail as a string
     * in the format 0 -> 1 -> ... -> n
     * @param g Graph
     * @param trail array of TrailStrucutre objects, returned by Sealib::GraphAlgorithms::hierholzer.
     * @return trail as string
     */
    static std::string stringFromTrail(Sealib::Graph *g, Sealib::TrailStructure **trail);

    /**
     * Writes a trail as dotfile
     * @param g Graph
     * @param trail array of TrailStrucutre objects, returned by Sealib::GraphAlgorithms::hierholzer.
     * @param fileName name of the file to write to
     */
    static void dotFileFromTrail(Sealib::Graph *g, Sealib::TrailStructure **trail, std::string fileName);

    /**
     * Writes a Sealib::Graph as dot file
     * @param g Graph
     * @param fileName name of the file to write to
     */
    static void dotFileFromGraph(Sealib::Graph *g, std::string fileName);

    /**
     * Converts a Sealib::Graph to an ogdf::Graph
     * @param g Sealib::Graph
     * @return ogdf::Graph pointer
     */
    static ogdf::Graph* ogdfGraphFromSealibGraph(Sealib::Graph *g);

    /**
     * Sets the attributes in an ogdf::GraphAttributes object based on a trail.
     * @param GA ogdf::GraphAttributes to set the attributes
     * @param g Graph
     * @param trail array of TrailStrucutre objects, returned by Sealib::GraphAlgorithms::hierholzer.
     * @return trail as string
     */
    static void graphAttributesFromTrail(ogdf::GraphAttributes*GA, Sealib::Graph *g, Sealib::TrailStructure **trail);

    /**
     * Converts an ogdf::Graph to a Sealib::Graph
     * @param g ogdf::Graph
     * @return Sealib::Graph
     */
    static Sealib::Graph* sealibGraphFromOgdfGraph(ogdf::Graph *g);

    /**
     * Creates a random ogdf::Graph that has exactly one euler trail with
     * nodeMax nodes and edgeMax edges.
     * @param nodeMax amount of nodes
     * @param edgeMax amount of edges
     * @return ogdf::Graph
     */
    static ogdf::Graph *randomEulerianOgdfGrah(int nodeMax, int edgeMax);

    /**
    * Creates a random Sealib::Graph that has exactly one euler trail with
    * nodeMax nodes and edgeMax edges.
    * @param nodeMax amount of nodes
    * @param edgeMax amount of edges
    * @return Sealib::Graph
    */
    static Sealib::Graph *randomEulerianSealibGraph(int nodeMax, int edgeMax);
};
}  // namespace Sealib
#endif  // SEA_GRAPHIO_H
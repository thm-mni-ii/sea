#ifndef SEALIB_GRAPH_GRAPHIO_H_
#define SEALIB_GRAPH_GRAPHIO_H_

#include <string>
#include "sealib/graph/graph.h"

namespace Sealib {
/**
 * Utility class to export graphs to several formats.
 */
class GraphExporter {
 public:
    /**
     * Exports a graph to GML.
     * @param g the graph to export
     * @param filename target file name
     */
    static void exportGML(Graph *g, bool directed, std::string filename);
};

class GraphImporter {
 public:
    /**
     * Import a graph from a GML file.
     * @param filename the source file name
     * @return graph of the desired type G
     */
    template<class G>
    static G importGML(std::string filename);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPH_GRAPHIO_H_

#ifndef SEALIB_GRAPHIO_H_
#define SEALIB_GRAPHIO_H_

#include <string>
#include "sealib/graph.h"

namespace Sealib {
class GraphExporter {
 public:
    static void exportGML(Graph *g, std::string filename);
};

class GraphImporter {
 public:
    template <class G>
    static G importGML(std::string filename);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPHIO_H_

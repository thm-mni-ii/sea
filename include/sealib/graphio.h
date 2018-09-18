#ifndef SEALIB_GRAPHIO_H_
#define SEALIB_GRAPHIO_H_

#include <fstream>
#include "sealib/graph.h"

namespace Sealib {
class GraphExporter {
 public:
  static void exportGML(Graph *g, std::string filename);
};
}  // namespace Sealib
#endif  // SEALIB_GRAPHIO_H_

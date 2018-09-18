#include "sealib/graphio.h"

using namespace Sealib;  // NOLINT

void GraphExporter::exportGML(Graph *g, std::string filename) {
  std::ofstream out(filename);
  out << "graph\n[\ndirected 1\n";
  for (unsigned u = 0; u < g->getOrder(); u++) {
    out << "node\n[\nid " << u << "\n";
    out << "graphics\n[\n";
    out << "x 0.0\ny 0.0\nz 0.0\n";
    out << "w 10.0\nh 10.0\nd 10.0\n";
    out << "fill \"#000000\"\n";
    out << "]\n";
    out << "]\n";
  }
  unsigned edgeId = g->getOrder();
  for (unsigned u = 0; u < g->getOrder(); u++) {
    for (unsigned k = 0; k < g->getNodeDegree(u); k++) {
      out << "edge\n[\nid " << edgeId++ << "\n";
      out << "source " << u << "\n";
      out << "target " << g->head(u, k) << "\n";
      out << "value 1.0\n";
      out << "fill \"#000000\"";
      out << "]\n";
    }
  }
  out << "]\n";
  out.close();
}

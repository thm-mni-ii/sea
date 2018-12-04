#ifndef SEALIBVISUAL_EXAMPLES_H_
#define SEALIBVISUAL_EXAMPLES_H_
#include <sealib/collection/compactarray.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/iterator/bfs.h>
#include <sealib/iterator/dfs.h>
#include <sealib/iterator/edgemarker.h>
#include <sealib/segmentstack.h>
#include <string>
#include "./tikzdocument.h"
#include "./tikzgraph.h"
#include "./tikzpicture.h"

/**
 * This file contains example algorithms, animated with TEX output.
 */

namespace SealibVisual {
class Examples {
 public:
    static const char *style_white, *style_lightgray, *style_gray, *style_black;
};

/**
 * BFS visualisation.
 */
class VisualBFS {
 public:
    /**
     * Creates a new BFS visualisation.
     * @param Graph Graph to use
     * @param CompactArray Color array to use
     * @param filename Output file name
     * @param mode Output mode: "standalone" or "beamer"
     */
    VisualBFS(Sealib::Graph *, Sealib::CompactArray,
              std::string filename = "example.tex",
              std::string mode = "standalone");
    /**
     * Runs the visualisation and creates the TEX output.
     */
    void run();

 private:
    Sealib::Graph *g;
    std::shared_ptr<TikzGraph> tg;
    Sealib::CompactArray c;
    TikzDocument doc;
    std::shared_ptr<TikzPicture> pic;

    void emit();
};

class VisualDFS : Sealib::ExtendedSegmentStack, Sealib::DFS {
 public:
    /**
     * Creates a new DFS visualisation.
     * @param Graph Graph to use
     * @param CompactArray Color array to use
     * @param filename Output file name
     * @param mode Output mode: "standalone" or "beamer"
     */
    VisualDFS(Sealib::Graph *, Sealib::CompactArray *,
              std::string filename = "example.tex",
              std::string mode = "standalone");
    /**
     * Runs the visualisation and creates the TEX output.
     */
    void run();

 private:
    Sealib::Graph *g;
    std::shared_ptr<TikzGraph> tg;
    Sealib::CompactArray *c;
    TikzDocument doc;
    std::shared_ptr<TikzPicture> pic;

    void emit();
};

class VisualEdgeMarker : public Sealib::EdgeMarker {
 public:
   explicit VisualEdgeMarker(Sealib::UndirectedGraph *, 
      std::string filename = "example.tex",
      std::string mode = "standalone");

   void initEdge(uint u, uint k, uint8_t type) override;

   void setMark(uint u, uint k, uint8_t mark) override;

   ~VisualEdgeMarker() {
      doc.close();
   }

 private:
   std::shared_ptr<TikzGraph> tg;
   std::shared_ptr<TikzPicture> pic;
   TikzDocument doc;

   void emit();

   std::string getStyle(uint u, uint k);
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_EXAMPLES_H_

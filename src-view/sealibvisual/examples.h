#ifndef SEALIBVISUAL_EXAMPLES_H_
#define SEALIBVISUAL_EXAMPLES_H_
#include <sealib/iterator/bfs.h>
#include <sealib/collection/compactarray.h>
#include <sealib/iterator/dfs.h>
#include <sealib/segmentstack.h>
#include <sealib/graph/directedgraph.h>
#include "./tikzdocument.h"
#include "./tikzgraph.h"
#include "./tikzpicture.h"
#include <string>

namespace SealibVisual {
class Examples {
 public:
  static const char *style_white, *style_lightgray, *style_gray, *style_black;
};

class VisualBFS {
 private:
  Sealib::Graph *g;
  std::shared_ptr<TikzGraph> tg;
  Sealib::CompactArray c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;

 public:
  VisualBFS(Sealib::Graph *, Sealib::CompactArray,
            std::string filename = "example.tex",
            std::string mode = "standalone");
  void run();
  void emit();
};

class VisualDFS : Sealib::ExtendedSegmentStack, Sealib::DFS {
 private:
  Sealib::Graph *g;
  std::shared_ptr<TikzGraph> tg;
  Sealib::CompactArray *c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;

 public:
  VisualDFS(Sealib::Graph *, Sealib::CompactArray *,
            std::string filename = "example.tex",
            std::string mode = "standalone");
  void run();
  void emit();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_EXAMPLES_H_

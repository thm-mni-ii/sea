#ifndef SEALIBVISUAL_EXAMPLES_H_
#define SEALIBVISUAL_EXAMPLES_H_
#include <sealib/basicgraph.h>
#include <sealib/bfs.h>
#include <sealib/compactarray.h>
#include <sealib/dfs.h>
#include <sealib/segmentstack.h>
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
  Sealib::BasicGraph *g;
  Sealib::CompactArray *c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;
  std::shared_ptr<TikzGraph> tg;

 public:
  VisualBFS(Sealib::BasicGraph *, Sealib::CompactArray *,
            std::string filename = "example.tex",
            std::string mode = "standalone");
  void run();
  void emit();
};

class VisualDFS : Sealib::ExtendedSegmentStack, Sealib::DFS {
 private:
  Sealib::BasicGraph *g;
  Sealib::CompactArray *c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;
  std::shared_ptr<TikzGraph> tg;

 public:
  VisualDFS(Sealib::BasicGraph *, Sealib::CompactArray *,
            std::string filename = "example.tex",
            std::string mode = "standalone");
  void run();
  void emit();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_EXAMPLES_H_

#ifndef SEALIBVISUAL_EXAMPLES_H_
#define SEALIBVISUAL_EXAMPLES_H_
#include <sealibvisual/tikzdocument.h>
#include <sealibvisual/tikzgraph.h>
#include <sealibvisual/tikzpicture.h>
#include <string>

namespace SealibVisual {
/**
 * Examples is a friend class of: Sealib::SegmentStack, Sealib::DFS
 */
class Examples {
 public:
  static void visualBFS(std::string filename = "example.tex");

  static void visualDFS(std::string filename = "example.tex");

  static const char *style_white, *style_lightgray, *style_gray, *style_black;
};

class VisualDFS {
 private:
  Sealib::Graph *g;
  Sealib::SegmentStack *s;
  Sealib::CompactArray *c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;
  std::shared_ptr<TikzGraph> tg;

 public:
  VisualDFS(Sealib::Graph *, Sealib::CompactArray *, Sealib::SegmentStack *,
            std::string filename = "example.tex");
  void run();
  void preprocess(uint);
  void postprocess(uint);
  void emit();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_EXAMPLES_H_

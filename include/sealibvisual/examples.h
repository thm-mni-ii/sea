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

class VisualBFS {
 private:
  Sealib::Graph *g;
  Sealib::CompactArray *c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;
  std::shared_ptr<TikzGraph> tg;

 public:
  VisualBFS(Sealib::Graph *, Sealib::CompactArray *,
            std::string filename = "example.tex");
  void run();
  void emit();
};

class VisualDFS {
 private:
  Sealib::Graph *g;
  Sealib::ExtendedSegmentStack *s;
  Sealib::CompactArray *c;
  TikzDocument *doc;
  std::shared_ptr<TikzPicture> pic;
  std::shared_ptr<TikzGraph> tg;

 public:
  VisualDFS(Sealib::Graph *, Sealib::CompactArray *,
            Sealib::ExtendedSegmentStack *,
            std::string filename = "example.tex");
  void run();
  void emit();
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_EXAMPLES_H_

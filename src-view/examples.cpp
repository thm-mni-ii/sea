#include "sealibvisual/examples.h"

#include <iostream>

#include "sealib/graph/undirectedgraph.h"
#include "sealib/iterator/bfs.h"
#include "sealib/iterator/dfs.h"
#include "sealib/graph/graphcreator.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealibvisual/tikzgraph.h"

using namespace Sealib;   // NOLINT

namespace SealibVisual {

const char *Examples::style_white = "circle,draw=black,fill=white",
           *Examples::style_lightgray = "circle,draw=black,fill=gray!50",
           *Examples::style_gray = "circle,draw=black,fill=gray",
           *Examples::style_black = "circle,text=white,fill=black";

//  --- VISUAL BREADTH-FIRST SEARCH ---

VisualBFS::VisualBFS(UndirectedGraph *graph, CompactArray *color, std::string filename,
                     std::string mode)
    : g(graph),
      tg(TikzGenerator::generateTikzElement(*g, true)),
      c(color),
      doc(new TikzDocument(filename, "matrix,graphdrawing,positioning",
                           "layered,force", true, mode)),
      pic(new TikzPicture(
          "spring electrical layout, sibling distance=15mm, node "
          "distance=20mm, node sep=1cm, arrows={->}, line "
          "width=1pt, color=black")) {
  pic->add(tg);
}

void VisualBFS::emit() {
  doc->beginBlock();
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*c, "color", "yshift=-8cm"));
  doc->endBlock();
}

void VisualBFS::run() {
  BFS b(g, c,
        [this](uint u) {
          tg->getNodes().at(u).setOptions(Examples::style_lightgray);
          emit();
        },
        [this](uint u, uint) {
          if (tg->getNodes().at(u).getOptions() != Examples::style_gray) {
            tg->getNodes().at(u).setOptions(Examples::style_gray);
            emit();
          }
        });
  b.init();
  do {
    while (b.more()) {
      std::pair<uint, uint> p = b.next();
      tg->getNodes().at(p.first).setOptions(Examples::style_black);
      emit();
    }
  } while (b.nextComponent());
  doc->close();
}

// --- VISUAL DEPTH-FIRST SEARCH

VisualDFS::VisualDFS(UndirectedGraph *graph, CompactArray *color, std::string filename,
                     std::string mode)
    : ExtendedSegmentStack(graph->getOrder(), graph, color),
      g(graph),
      tg(TikzGenerator::generateTikzElement(*g, true)),
      c(color),
      doc(new TikzDocument(filename, "matrix,graphdrawing,positioning",
                           "layered,force", true, mode)),
      pic(new TikzPicture(
          "spring electrical layout, sibling distance=15mm, node "
          "distance=20mm, node sep=1cm, arrows={->}, line "
          "width=1pt, color=black")) {
  pic->add(tg);
}

void VisualDFS::emit() {
  doc->beginBlock();
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*c, "color", "yshift=-8cm"));
  std::vector<uint> l, h, t;
  for (uint a = 0; a < lp; a++) l.push_back(low[a].first);
  for (uint a = 0; a < hp; a++) h.push_back(high[a].first);
  for (uint a = 0; a < tp; a++) t.push_back(trailers[a].x.first);
  doc->add(TikzGenerator::generateTikzElement(
      l, "$S_L$", true, "yshift=-8cm, xshift=8cm, name=SL"));
  doc->add(TikzGenerator::generateTikzElement(
      h, "$S_H$", true, "yshift=-8cm, xshift=10cm, name=SH"));
  doc->add(TikzGenerator::generateTikzElement(
      t, "T", true, "yshift=-8cm, xshift=12cm, name=T"));
  doc->endBlock();
}

void VisualDFS::run() {
  for (uint u = 0; u < g->getOrder(); u++) {
    if (c->get(u) == DFS_WHITE) {
      process_small<ExtendedSegmentStack>(
          u, g, c, this, restore_top,
          [this](uint u) {
            tg->getNodes().at(u).setOptions(Examples::style_gray);
            emit();
          },
          DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
          [this](uint u) {
            tg->getNodes().at(u).setOptions(Examples::style_black);
            emit();
          });
    }
  }
  doc->close();
}

}   // namespace SealibVisual

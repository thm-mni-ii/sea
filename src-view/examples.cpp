#include <sealib/basicgraph.h>
#include <sealib/bfs.h>
#include <sealib/dfs.h>
#include <sealib/graphcreator.h>
#include <sealibvisual/examples.h>
#include <sealibvisual/tikzgenerator.h>
#include <sealibvisual/tikzgraph.h>
#include <iostream>
using SealibVisual::Examples;
using SealibVisual::VisualDFS;
using SealibVisual::VisualBFS;
using SealibVisual::TikzDocument;
using SealibVisual::TikzPicture;
using SealibVisual::TikzGenerator;
using SealibVisual::TikzGraph;
using Sealib::Graph;
using Sealib::BasicGraph;
using Sealib::BFS;
using Sealib::DFS;
using Sealib::CompactArray;
using Sealib::ExtendedSegmentStack;

const char *Examples::style_white = "circle,draw=black,fill=white",
           *Examples::style_lightgray = "circle,draw=black,fill=gray!50",
           *Examples::style_gray = "circle,draw=black,fill=gray",
           *Examples::style_black = "circle,text=white,fill=black";

//  --- VISUAL BREADTH-FIRST SEARCH ---

VisualBFS::VisualBFS(BasicGraph *g, CompactArray *c, std::string filename,
                     std::string mode)
    : g(g),
      tg(TikzGenerator::generateTikzElement(*g, true)),
      c(c),
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
        [this](uint u, uint v) {
          if (tg->getNodes().at(u).getOptions() != Examples::style_gray) {
            tg->getNodes().at(u).setOptions(Examples::style_gray);
            emit();
          }
        });
  b.init();
  do {
    while (b.more()) {
      Pair p = b.next();
      tg->getNodes().at(p.head()).setOptions(Examples::style_black);
      emit();
    }
  } while (b.nextComponent());
  doc->close();
}

// --- VISUAL DEPTH-FIRST SEARCH

VisualDFS::VisualDFS(BasicGraph *g, CompactArray *c, std::string filename,
                     std::string mode)
    : ExtendedSegmentStack(g->getOrder(), g, c),
      g(g),
      tg(TikzGenerator::generateTikzElement(*g, true)),
      c(c),
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
  for (uint a = 0; a < lp; a++) l.push_back(low[a].head());
  for (uint a = 0; a < hp; a++) h.push_back(high[a].head());
  for (uint a = 0; a < tp; a++) t.push_back(trailers[a].x.head());
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

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

VisualBFS::VisualBFS(Graph *g, CompactArray *c, std::string filename)
    : g(g),
      tg(TikzGenerator::generateTikzElement(*static_cast<BasicGraph *>(g),
                                            true)),
      c(c),
      doc(new TikzDocument(filename, "matrix,graphdrawing,positioning",
                           "layered,force", true)),
      pic(new TikzPicture("spring layout, sibling distance=15mm, node "
                          "distance=20mm, node sep=1cm, arrows={->}, line "
                          "width=1pt, color=black")) {
  pic->add(tg);
}

void VisualBFS::emit() {
  doc->beginBlock();
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*c, "color"));
  doc->endBlock();
}

void VisualBFS::run() {
  BFS b(g, c,
        [this](uint u) {
          emit();
          tg->getNodes().at(u).setOptions(Examples::style_lightgray);
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

VisualDFS::VisualDFS(Graph *g, CompactArray *c, ExtendedSegmentStack *s,
                     std::string filename)
    : g(g),
      tg(TikzGenerator::generateTikzElement(*static_cast<BasicGraph *>(g),
                                            true)),
      s(s),
      c(c),
      doc(new TikzDocument(filename, "matrix,graphdrawing,positioning",
                           "layered,force", true)),
      pic(new TikzPicture("spring layout, sibling distance=15mm, node "
                          "distance=20mm, node sep=1cm, arrows={->}, line "
                          "width=1pt, color=black")) {
  pic->add(tg);
}

void VisualDFS::emit() {
  doc->beginBlock();
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*c, "color"));
  std::vector<uint> l, h, t;
  for (uint a = 0; a < s->lp; a++) l.push_back(s->low[a].head());
  for (uint a = 0; a < s->hp; a++) h.push_back(s->high[a].head());
  for (uint a = 0; a < s->tp; a++) t.push_back(s->trailers[a].x.head());
  doc->add(TikzGenerator::generateTikzElement(l, "$S_L$", true));
  doc->add(TikzGenerator::generateTikzElement(h, "$S_H$", true));
  doc->add(TikzGenerator::generateTikzElement(t, "T", true));
  doc->endBlock();
}

void VisualDFS::run() {
  for (uint u = 0; u < g->getOrder(); u++) {
    DFS::process_small(u, g, c, s, DFS::restore_top,
                       [this](uint u) {
                         emit();
                         tg->getNodes().at(u).setOptions(Examples::style_gray);
                       },
                       DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                       [this](uint u) {
                         tg->getNodes().at(u).setOptions(Examples::style_black);
                         emit();
                       });
  }
  doc->close();
}

// --- OLD SETUP:

static TikzDocument *doc = nullptr;
static std::shared_ptr<TikzPicture> pic;
static std::shared_ptr<TikzGraph> tg;

static CompactArray *ca;
static std::vector<uint> *s;

const char *style_white = "circle,draw=black,fill=white",
           *style_lightgray = "circle,draw=black,fill=gray!50",
           *style_gray = "circle,draw=black,fill=gray",
           *style_black = "circle,text=white,fill=black";

static unsigned bfs_inner = BFS_GRAY2, bfs_outer = BFS_GRAY1;

static void bfs_emit() {
  doc->beginBlock();
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*ca, "color"));
  doc->endBlock();
}

static void bfs_preprocess(uint u) {
  // std::cout << "preprocess " << u << "\n";
  tg->getNodes().at(u).setOptions(style_lightgray);
  ca->insert(u, bfs_outer);
  bfs_emit();
}
static void bfs_preexplore(uint u, uint v) {
  if (ca->get(u) != bfs_inner) {
    tg->getNodes().at(u).setOptions(style_gray);
    ca->insert(u, bfs_inner);
    bfs_emit();
  }
}

static void bfs_swap() {
  // swap gray if necessary:
  bool swap = true;
  for (uint f = 0; f < ca->size(); f++) {
    if (ca->get(f) == bfs_inner) {
      swap = false;
      break;
    }
  }
  if (swap) {
    for (uint f = 0; f < ca->size(); f++) {
      if (ca->get(f) == bfs_outer) {
        tg->getNodes().at(f).setOptions(style_gray);
      }
    }
    unsigned tmp = bfs_inner;
    bfs_inner = bfs_outer;
    bfs_outer = tmp;
  }
}

void Examples::visualBFS(std::string filename) {
  doc = new TikzDocument(filename, "matrix,graphdrawing,positioning",
                         "layered,force", true);
  pic = std::shared_ptr<TikzPicture>(new TikzPicture(
      "spring layout, sibling distance=15mm, node "
      "distance=20mm, node sep=1cm, line width=1pt, color=black"));
  uint n = 10;

  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 2);
  for (uint a = 0; a < n; a++) {
    std::cout << "node " << a << ": ";
    for (uint b = 0; b < g->getNodeDegree(a); b++)
      std::cout << g->head(a, b) << " ";
    std::cout << "\n";
  }
  tg = TikzGenerator::generateTikzElement(*g, true);
  ca = new CompactArray(n, 3);
  BFS b(g, bfs_preprocess, bfs_preexplore);

  pic->add(tg);
  b.init();

  do {
    while (b.more()) {
      Pair p = b.next();

      tg->getNodes().at(p.head()).setOptions(style_black);
      ca->insert(p.head(), BFS_BLACK);
      // bfs_swap();
      bfs_emit();
      std::cout << p.head() << "," << p.tail() << " ";
    }
    std::cout << "\n";
  } while (b.nextComponent());
  doc->close();
}

static void dfs_emit() {
  doc->beginBlock();
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*ca, "color"));
  doc->add(TikzGenerator::generateTikzElement(*s, "S", true));
  doc->endBlock();
}

static void dfs_preprocess(uint u) {
  tg->getNodes().at(u).setOptions(style_gray);
  ca->insert(u, DFS_GRAY);
  s->push_back(u);
  dfs_emit();
}

static void dfs_postprocess(uint u) {
  tg->getNodes().at(u).setOptions(style_black);
  ca->insert(u, DFS_BLACK);
  s->pop_back();
  dfs_emit();
}

void Examples::visualDFS(std::string filename) {
  doc = new TikzDocument(filename, "matrix,graphdrawing,positioning",
                         "layered,force", true);
  pic = std::shared_ptr<TikzPicture>(new TikzPicture(
      "spring layout, sibling distance=15mm, node "
      "distance=20mm, node sep=1cm, arrows={->}, line width=1pt, color=black"));

  uint n = 20;
  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 3);
  tg = TikzGenerator::generateTikzElement(*g, true);
  ca = new CompactArray(n, 3);
  s = new std::vector<uint>;

  pic->add(tg);

  DFS::nBitDFS(g, dfs_preprocess, nullptr, nullptr, dfs_postprocess);

  doc->close();
}

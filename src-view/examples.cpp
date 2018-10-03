#include <sealib/basicgraph.h>
#include <sealib/bfs.h>
#include <sealib/dfs.h>
#include <sealib/graphcreator.h>
#include <sealibvisual/examples.h>
#include <sealibvisual/tikzgenerator.h>
#include <sealibvisual/tikzgraph.h>
#include <iostream>
using SealibVisual::Examples;
using SealibVisual::TikzDocument;
using SealibVisual::TikzPicture;
using SealibVisual::TikzGenerator;
using SealibVisual::TikzGraph;
using Sealib::Graph;
using Sealib::BasicGraph;
using Sealib::BFS;
using Sealib::DFS;
using Sealib::CompactArray;

static TikzDocument *doc = nullptr;
static std::shared_ptr<TikzPicture> pic;
static std::shared_ptr<TikzGraph> tg;

static CompactArray *ca;
static std::vector<uint> *s;

const char *style_white = "circle,draw=black,fill=white",
           *style_lightgray = "circle,draw=black,fill=gray!50",
           *style_gray = "circle,draw=black,fill=gray",
           *style_black = "circle,draw=black,fill=black";

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
  pic = std::shared_ptr<TikzPicture>(
      new TikzPicture("spring layout, sibling distance=15mm, node "
                      "distance=20mm, node sep=1cm"));
  uint n = 10;

  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 2);
  for (uint a = 0; a < n; a++) {
    std::cout << "node " << a << ": ";
    for (uint b = 0; b < g->getNodeDegree(a); b++)
      std::cout << g->head(a, b) << " ";
    std::cout << "\n";
  }
  tg = TikzGenerator::generateTikzElement(*g);
  ca = new CompactArray(n, 3);
  BFS b(g, bfs_preprocess, bfs_preexplore);

  for (auto &e : tg->getEdges()) {
    e.second.setOptions("->, line width=1pt, color=black");
  }
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
  pic = std::shared_ptr<TikzPicture>(
      new TikzPicture("spring layout, sibling distance=15mm, node "
                      "distance=20mm, node sep=1cm"));

  uint n = 10;
  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 2);
  tg = TikzGenerator::generateTikzElement(*g);
  ca = new CompactArray(n, 3);
  s = new std::vector<uint>;

  for (auto &e : tg->getEdges()) {
    e.second.setOptions("->, line width=1pt, color=black");
  }
  pic->add(tg);

  DFS::nBitDFS(g, dfs_preprocess, nullptr, nullptr, dfs_postprocess);

  doc->close();
}

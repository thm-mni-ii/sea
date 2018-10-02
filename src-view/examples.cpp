#include <sealib/basicgraph.h>
#include <sealib/bfs.h>
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
using Sealib::CompactArray;

static TikzDocument *doc = nullptr;
static std::shared_ptr<TikzGraph> tg;
static std::shared_ptr<TikzPicture> pic;

static unsigned bfs_inner = BFS_GRAY1, bfs_outer = BFS_GRAY2;
static CompactArray *ca;

const char *style_white = "circle,draw=black,fill=white",
           *style_lightgray = "circle,draw=black,fill=gray!20",
           *style_gray = "circle,draw=black,fill=gray!75",
           *style_black = "circle,draw=black,fill=black";

static void bfs_emit() {
  doc->add(pic);
  doc->add(TikzGenerator::generateTikzElement(*ca, "color"));
  doc->add("\\newpage");
}

static void bfs_preprocess(uint u) {
  // std::cout << "preprocess " << u << "\n";
  tg->getNodes().at(u).setOptions(style_lightgray);
  ca->insert(u, bfs_outer);
  bfs_emit();
}
static void bfs_preexplore(uint u, uint v) {
  tg->getNodes().at(u).setOptions("circle,draw=black,fill=gray!70");
  ca->insert(v, bfs_inner);
  // bfs_emit();
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

TikzDocument *Examples::visualBFS(std::string filename) {
  doc =
      new TikzDocument(filename, "matrix,graphdrawing", "layered,force", true);
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
  BFS b(g, bfs_preprocess /*BFS_NOP_PROCESS*/,
        bfs_preexplore /*BFS_NOP_EXPLORE*/);

  for (auto &e : tg->getEdges()) {
    e.second.setOptions("->, color=black");
  }
  pic->add(tg);
  b.init();

  do {
    while (b.more()) {
      Pair p = b.next();

      tg->getNodes().at(p.head()).setOptions(style_black);
      ca->insert(p.head(), BFS_BLACK);
      bfs_swap();
      bfs_emit();
      std::cout << p.head() << "," << p.tail() << " ";
    }
    std::cout << "\n";
  } while (b.nextComponent());
  return doc;
}

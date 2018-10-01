#include <sealib/basicgraph.h>
#include <sealib/bfs.h>
#include <sealib/graphcreator.h>
#include <sealibvisual/examples.h>
#include <sealibvisual/tikzgenerator.h>
#include <sealibvisual/tikzgraph.h>
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

static std::shared_ptr<TikzGraph> bfs_vg;
static std::shared_ptr<TikzPicture> bfs_pic;
static uint bfs_inner = BFS_GRAY1, bfs_outer = BFS_GRAY2;
static CompactArray *bfs_c;

static void bfs_emit() {
  doc->add(bfs_pic);
  doc->add(TikzGenerator::generateTikzElement(*bfs_c, "color"));
  doc->add("\\newpage");
}

static void vbfs_preprocess(uint u) {
  bfs_vg->getNodes().at(u).setOptions("circle,draw=black,fill=gray!30");
  bfs_c->insert(u, bfs_outer);
  bfs_emit();
}
// static void vbfs_preexplore(uint u,uint v) {
//   bfs_vg->getNodes().at(v).setOptions("circle,draw=black,fill=gray!70");
//   bfs_c->insert(v, BFS_GRAY1);
//   doc->add(bfs_pic);
//   doc->add(TikzGenerator::generateTikzElement(*bfs_c, "color"));
//   doc->add("\\newpage");
// }

TikzDocument *Examples::visualBFS(std::string filename) {
  doc =
      new TikzDocument(filename, "matrix,graphdrawing", "layered,force", true);
  bfs_pic = std::shared_ptr<TikzPicture>(new TikzPicture("spring layout"));
  uint n = 10;
  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 1);
  bfs_vg = TikzGenerator::generateTikzElement(*g);
  for (auto &e : bfs_vg->getEdges()) {
    e.second.setOptions("->, color=black");
  }
  bfs_pic->add(bfs_vg);
  bfs_c = new CompactArray(n, 3);
  BFS b(g, vbfs_preprocess, BFS_NOP_EXPLORE);
  b.init();
  while (b.more() || b.nextComponent()) {
    uint u = b.next().head();
    if (bfs_c->get(u) == bfs_inner) {
      bfs_vg->getNodes().at(u).setOptions("circle,draw=white,fill=black");
      bfs_c->insert(u, BFS_BLACK);
      bfs_emit();
    } else {
      bool gi = false;
      for (uint d = 0; d < n; d++) {
        if (bfs_c->get(d) == bfs_inner) {
          gi = true;
          break;
        }
      }
      if (!gi) {
        for (uint d = 0; d < n; d++) {
          if (bfs_c->get(d) == bfs_outer) {
            bfs_c->insert(d, bfs_inner);
            bfs_vg->getNodes().at(d).setOptions(
                "circle,draw=black,fill=gray!70");
          }
        }
        uint tmp = bfs_inner;
        bfs_inner = bfs_outer;
        bfs_outer = tmp;
        bfs_emit();
      }
    }
  }
  return doc;
}

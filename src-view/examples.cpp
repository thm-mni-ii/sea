#include "sealibvisual/examples.h"

#include <map>
#include <string>
#include "sealib/graph/graphcreator.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealibvisual/tikzgraph.h"

using namespace Sealib;  // NOLINT

namespace SealibVisual {

char const *Examples::style_white = "circle,draw=black,fill=white",
           *Examples::style_lightgray = "circle,draw=black,fill=gray!50",
           *Examples::style_gray = "circle,draw=black,fill=gray",
           *Examples::style_black = "circle,text=white,fill=black";

//  --- VISUAL BREADTH-FIRST SEARCH ---

VisualBFS::VisualBFS(Graph *graph, CompactArray color, std::string filename,
                     std::string mode)
    : g(graph),
      tg(TikzGenerator::generateTikzElement(g)),
      c(std::move(color)),
      doc(filename, "matrix,graphdrawing,positioning", "layered,force", true,
          mode),
      pic(new TikzPicture(
          "spring electrical layout, sibling distance=15mm, node "
          "distance=20mm, node sep=1cm, arrows={->}, line "
          "width=1pt, color=black")) {
    pic->add(tg);
    emit();
}

void VisualBFS::emit() {
    doc.beginBlock();
    doc.add(pic);
    doc.endBlock();
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
            bool haveGray = false;
            for (TikzNode &node : tg->getNodes()) {
                if (node.getOptions() == Examples::style_gray) {
                    haveGray = true;
                    break;
                }
            }
            if (!haveGray) {
                bool doEmit = false;
                for (TikzNode &node : tg->getNodes()) {
                    if (node.getOptions() == Examples::style_lightgray) {
                        node.setOptions(Examples::style_gray);
                        doEmit = true;
                    }
                }
                if (doEmit) emit();
            }
        }
    } while (b.nextComponent());
    doc.close();
}

// --- VISUAL DEPTH-FIRST SEARCH ---

VisualDFS::VisualDFS(Graph *graph, CompactArray *color, std::string filename,
                     std::string mode)
    : ExtendedSegmentStack(graph->getOrder(), graph, color),
      g(graph),
      tg(TikzGenerator::generateTikzElement(g)),
      c(color),
      doc(filename, "matrix,graphdrawing,positioning", "trees", true, mode),
      pic(new TikzPicture("tree layout, sibling distance=15mm, node "
                          "distance=20mm, node sep=1cm, arrows={->}, line "
                          "width=1pt, color=black")) {
    pic->add(tg);
    emit();
}

void VisualDFS::emit() {
    doc.beginBlock();
    doc.add(pic);
    std::vector<uint> l, h, t;
    for (uint a = 0; a < lp; a++) l.push_back(low[a].first);
    for (uint a = 0; a < hp; a++) h.push_back(high[a].first);
    for (uint a = 0; a < tp; a++) t.push_back(trailers[a].x.first);
    doc.add(TikzGenerator::generateTikzElement(
        l, "$S_L$", true, "yshift=-8cm, xshift=10cm, name=SL"));
    doc.add(TikzGenerator::generateTikzElement(
        h, "$S_H$", true, "yshift=-8cm, xshift=12cm, name=SH"));
    doc.add(TikzGenerator::generateTikzElement(
        t, "T", true, "yshift=-8cm, xshift=14cm, name=T"));
    doc.endBlock();
}

void VisualDFS::run() {
    for (uint u = 0; u < g->getOrder(); u++) {
        if (c->get(u) == DFS_WHITE) {
            visit_nloglogn<ExtendedSegmentStack>(
                u, reinterpret_cast<Graph *>(g), c, this, restore_top,
                [this](uint ui) {
                    tg->getNodes().at(ui).setOptions(Examples::style_gray);
                    emit();
                },
                DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                [this](uint ui) {
                    tg->getNodes().at(ui).setOptions(Examples::style_black);
                    emit();
                });
        }
    }
    doc.close();
}

// --- VISUAL EDGE MARKER ---

VisualEdgeMarker::VisualEdgeMarker(UndirectedGraph *graph, std::string filename,
                                   std::string mode, bool flagSilent)
    : EdgeMarker(graph),
      doc(filename, "matrix,graphdrawing,positioning,quotes", "layered,force",
          true, mode),
      pic(new TikzPicture(
          "spring electrical layout, sibling distance=15mm, node "
          "distance=17mm, node sep=1cm, arrows={->}, line "
          "width=1pt, color=black")),
      tg(TikzGenerator::generateTikzElement(g)),
      silent(flagSilent) {
    pic->add(tg);
}

VisualEdgeMarker::~VisualEdgeMarker() { doc.close(); }

void VisualEdgeMarker::emit() {
    doc.beginBlock();
    doc.add(pic);
    doc.endBlock();
}

std::string VisualEdgeMarker::getStyle(uint u, uint k) {
    std::stringstream options;
    uint v = g->head(u, k);
    if (isParent(u, k)) {
        options << (u < v ? "->" : "<-");
    } else if (!isParent(u, k)) {
        options << (u < v ? "<-" : "->");
    } else {
        options << "--";
    }
    switch (getEdgeData(u, k) & TYPE_MASK) {
        case FULL:
            options << ",blue,solid";
            break;
        case HALF:
            options << ",blue,densely dashed";
            break;
        case UNMARKED:
            options << ",blue,dotted";
            break;
        case BACK:
            options << ",dotted";
        case CROSS:
            options << ",gray,thin";
        default:
            break;
    }
    return options.str();
}

void VisualEdgeMarker::updateEdge(uint u, uint k) {
    using key_t = std::tuple<std::string, std::string>;
    std::string a = std::to_string(u), b = std::to_string(g->head(u, k));
    auto edge = tg->getEdges().find(key_t{a, b});
    if (edge != tg->getEdges().end()) {
        edge->second.setOptions(getStyle(u, k));
    } else {
        auto edgeReverse = tg->getEdges().find(key_t{b, a});
        assert(edgeReverse != tg->getEdges().end());
        edgeReverse->second.setOptions(getStyle(u, k));
    }
}

void VisualEdgeMarker::initEdge(uint u, uint k, uint8_t type) {
    EdgeMarker::initEdge(u, k, type);
    updateEdge(u, k);
    if (!silent) emit();
}

void VisualEdgeMarker::setMark(uint u, uint k, uint8_t mark) {
    EdgeMarker::setMark(u, k, mark);
    updateEdge(u, k);
    if (!silent) emit();
}

// --- VISUAL CUT-VERTEX ITERATOR ---

VisualCutVertex::VisualCutVertex(std::shared_ptr<VisualEdgeMarker> edges)
    : CutVertexIterator(edges), e(edges) {
    emit();
}

void VisualCutVertex::emit() {
    e->doc.beginBlock();
    e->doc.add(e->pic);
    e->doc.endBlock();
}

uint VisualCutVertex::next() {
    uint r = CutVertexIterator::next();
    std::cout << r << " is a cut vertex\n";
    e->tg->getNodes()[r].setOptions("circle,draw=blue,double");
    emit();
    return r;
}

// --- VISUAL BICONNECTED-COMPONENT ITERATOR ---

VisualBCC::VisualBCC(std::shared_ptr<VisualEdgeMarker> edges)
    : BCCIterator(edges), e(edges) {
    emit();
}

void VisualBCC::emit() {
    e->doc.beginBlock();
    e->doc.add(e->pic);
    e->doc.endBlock();
}

std::pair<uint, uint> VisualBCC::next() {
    std::pair<uint, uint> r = BCCIterator::next();
    if (r.second == INVALID) {
        e->tg->getNodes()[r.first].setOptions("circle,draw=red");
    } else {
        using key_t = std::tuple<std::string, std::string>;
        std::string a = std::to_string(r.first), b = std::to_string(r.second);
        auto edge = e->tg->getEdges().find(key_t{a, b});
        if (edge != e->tg->getEdges().end()) {
            edge->second.setOptions("--,draw=red,solid");
        } else {
            auto edgeReverse = e->tg->getEdges().find(key_t{b, a});
            assert(edgeReverse != e->tg->getEdges().end());
            edgeReverse->second.setOptions("--,draw=red,solid");
        }
    }
    emit();
    return r;
}

}  // namespace SealibVisual

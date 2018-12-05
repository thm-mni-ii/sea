#include "sealibvisual/examples.h"

#include <iostream>
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
}

void VisualBFS::emit() {
    doc.beginBlock();
    doc.add(pic);
    doc.add(TikzGenerator::generateTikzElement(&c, g->getOrder(), "color",
                                               "yshift=-8cm"));
    doc.endBlock();
}

void VisualBFS::run() {
    BFS b(reinterpret_cast<Graph *>(g), c,
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
    doc.close();
}

// --- VISUAL DEPTH-FIRST SEARCH ---

VisualDFS::VisualDFS(Graph *graph, CompactArray *color, std::string filename,
                     std::string mode)
    : ExtendedSegmentStack(graph->getOrder(), graph, color),
      g(graph),
      tg(TikzGenerator::generateTikzElement(g)),
      c(color),
      doc(filename, "matrix,graphdrawing,positioning", "layered,force", true,
          mode),
      pic(new TikzPicture(
          "spring electrical layout, sibling distance=15mm, node "
          "distance=20mm, node sep=1cm, arrows={->}, line "
          "width=1pt, color=black")) {
    pic->add(tg);
}

void VisualDFS::emit() {
    doc.beginBlock();
    doc.add(pic);
    doc.add(TikzGenerator::generateTikzElement(c, g->getOrder(), "color",
                                               "yshift=-8cm"));
    std::vector<uint> l, h, t;
    for (uint a = 0; a < lp; a++) l.push_back(low[a].first);
    for (uint a = 0; a < hp; a++) h.push_back(high[a].first);
    for (uint a = 0; a < tp; a++) t.push_back(trailers[a].x.first);
    doc.add(TikzGenerator::generateTikzElement(
        l, "$S_L$", true, "yshift=-8cm, xshift=8cm, name=SL"));
    doc.add(TikzGenerator::generateTikzElement(
        h, "$S_H$", true, "yshift=-8cm, xshift=10cm, name=SH"));
    doc.add(TikzGenerator::generateTikzElement(
        t, "T", true, "yshift=-8cm, xshift=12cm, name=T"));
    doc.endBlock();
}

void VisualDFS::run() {
    for (uint u = 0; u < g->getOrder(); u++) {
        if (c->get(u) == DFS_WHITE) {
            process_small<ExtendedSegmentStack>(
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

VisualEdgeMarker::VisualEdgeMarker(UndirectedGraph *graph, TikzDocument *_doc,
                                   std::shared_ptr<TikzPicture> _pic,
                                   std::shared_ptr<TikzGraph> _tg)
    : EdgeMarker(graph), doc(_doc), pic(_pic), tg(_tg) {
    emit();
}

void VisualEdgeMarker::emit() {
    doc->beginBlock();
    doc->add(pic);
    doc->endBlock();
}

std::string VisualEdgeMarker::getStyle(uint u, uint k) {
    std::stringstream options;
    options << "--";
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
            options << ",densely dotted";
        case CROSS:
            options << ",gray,thin";
        default:
            break;
    }
    return options.str();
}

void VisualEdgeMarker::updateEdge(uint u, uint k) {
    std::string a = std::to_string(u), b = std::to_string(g->head(u, k));
    auto edge = tg->getEdges().find({a, b});
    if (edge != tg->getEdges().end()) {
        edge->second.setOptions(getStyle(u, k));
    } else {
        auto edgeReverse = tg->getEdges().find({b, a});
        assert(edgeReverse != tg->getEdges().end());
        edgeReverse->second.setOptions(getStyle(u, k));
    }
}

void VisualEdgeMarker::initEdge(uint u, uint k, uint8_t type) {
    if (k != INVALID) {  // ???
        EdgeMarker::initEdge(u, k, type);
        std::cout << "initializing " << std::to_string(u) << ","
                  << std::to_string(g->head(u, k)) << "\n";
        updateEdge(u, k);
        emit();
    }
}

void VisualEdgeMarker::setMark(uint u, uint k, uint8_t mark) {
    if (k != INVALID) {  // ???
        EdgeMarker::setMark(u, k, mark);
        std::string a = std::to_string(u), b = std::to_string(g->head(u, k));
        updateEdge(u, k);
        emit();
    }
}

// --- VISUAL CUT-VERTEX ITERATOR ---

VisualCutVertex::VisualCutVertex(std::shared_ptr<VisualEdgeMarker> edges)
    : CutVertexIterator(edges), e(edges) {}

void VisualCutVertex::emit(uint u) {
    e->doc->beginBlock();
    std::cout << u << " is a cut vertex\n";
    e->tg->getNodes()[u].setOptions("circle,draw=blue,double");
    e->doc->add(e->pic);
    e->doc->endBlock();
}

uint VisualCutVertex::next() {
    uint r = CutVertexIterator::next();
    emit(r);
    return r;
}

}  // namespace SealibVisual

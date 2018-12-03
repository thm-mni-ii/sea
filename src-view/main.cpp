#include <sealib/_types.h>
#include <sealib/graph/compactgraph.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/graph/graphrepresentations.h>
#include <sealib/iterator/choicedictionaryiterator.h>
#include <sealib/iterator/dfs.h>
#include <sealib/runtimetest.h>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include "sealibvisual/examples.h"
#include "sealibvisual/tikzarray.h"
#include "sealibvisual/tikzdocument.h"
#include "sealibvisual/tikzfigure.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealibvisual/tikzgraph.h"
#include "sealibvisual/tikznode.h"
#include "sealibvisual/tikzpicture.h"
#include "sealibvisual/tikzstyle.h"
#include "./test_visual.h"

using std::cout;
using std::stack;
using namespace Sealib;  // NOLINT
using namespace SealibVisual;   // NOLINT

void tikz_example() {}

void tikz_example2() {
    unsigned int order = 4;
    auto **adj_mtrx = (unsigned int **)malloc(sizeof(unsigned int) * 4 * 4);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};

    Sealib::UndirectedGraph bg =
        Sealib::GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
    std::shared_ptr<SealibVisual::TikzElement> vg =
        SealibVisual::TikzGenerator::generateTikzElement(&bg);

    std::shared_ptr<SealibVisual::TikzPicture> pic(
        new SealibVisual::TikzPicture(
            "spring layout, node distance=100pt, scale=1"));

    pic->add(vg);

    std::shared_ptr<SealibVisual::TikzFigure> fig(
        new SealibVisual::TikzFigure("A caption.", pic));

    SealibVisual::TikzDocument doc(
        "out.tex", "matrix, backgrounds, graphdrawing", "force", true);
    doc.add(fig);

    for (auto &e : ((SealibVisual::TikzGraph *)(vg.get()))->getEdges()) {
        e.second.setOptions("--, color=green, line width=8");
    }
    doc.add(fig);

    std::vector<std::string> numbers1(10);
    for (unsigned int i = 0; i < 10; i++) {
        numbers1[i] = std::to_string(i * 45);
    }

    Sealib::Bitset<unsigned char> bits(10);
    bits[4] = 1;

    std::shared_ptr<SealibVisual::TikzElement> tikzNode =
        SealibVisual::TikzGenerator::generateTikzElement(bits);
    pic->add(tikzNode);

    doc.add(fig);

    doc.close();
}

/*void tikz_example3() {
    DirectedGraph g = Sealib::GraphCreator::createRandomKRegularGraph(10, 4);
    std::shared_ptr<TikzGraph> vg = TikzGenerator::generateTikzElement(&g);
    TikzGraph *tg = vg.get();
    for (auto &e : tg->getEdges()) {
        e.second.setOptions("line width=1pt, color=black, arrows={->}");
    }

    Sealib::CompactArray c(10);
    std::shared_ptr<TikzPicture> vc =
        TikzGenerator::generateTikzElement(c, 10, "color");

    std::shared_ptr<TikzPicture> pic1(new TikzPicture("spring layout"));
    std::shared_ptr<TikzPicture> pic2(new TikzPicture(""));

    pic1->add(vg);

    TikzDocument doc("out-bfs.tex", "matrix,positioning,graphdrawing",
                     "layered,force", true);
    doc.add(pic1);
    // doc.add(pic2);
    doc.add(vc);
    doc.add("\\newpage");

    tg->getNodes().at(0).setOptions("circle,draw=black,fill=gray");
    c.insert(0, 1);
    doc.add(pic1);
    doc.add(TikzGenerator::generateTikzElement(c, 10, "color"));
    doc.add("\\newpage");

    tg->getNodes().at(0).setOptions("circle,draw=black,fill=gray");
    c.insert(0, 1);
    doc.add(pic1);
    doc.add(TikzGenerator::generateTikzElement(c, 10, "color"));
    doc.add("\\newpage");

    tg->getNodes().at(1).setOptions("circle,draw=black,fill=gray!30");
    tg->getNodes().at(5).setOptions("circle,draw=black,fill=gray!30");
    tg->getNodes().at(7).setOptions("circle,draw=black,fill=gray!30");
    tg->getNodes().at(8).setOptions("circle,draw=black,fill=gray!30");
    c.insert(1, 2);
    c.insert(5, 2);
    c.insert(7, 2);
    c.insert(8, 2);
    doc.add(pic1);
    doc.add(TikzGenerator::generateTikzElement(c, 10, "color"));
    doc.add("\\newpage");

    tg->getNodes().at(0).setOptions("circle,draw=black,fill=black");
    tg->getNodes().at(1).setOptions("circle,draw=black,fill=gray");
    tg->getNodes().at(5).setOptions("circle,draw=black,fill=gray");
    tg->getNodes().at(7).setOptions("circle,draw=black,fill=gray");
    tg->getNodes().at(8).setOptions("circle,draw=black,fill=gray");
    c.insert(0, 3);
    c.insert(1, 1);
    c.insert(5, 1);
    c.insert(7, 1);
    c.insert(8, 1);
    doc.add(pic1);
    doc.add(TikzGenerator::generateTikzElement(c, 10, "color"));
    doc.add("\\newpage");

    doc.close();

    // Convert PDF to GIF:
    // convert -density 200 -delay 100 -background white -alpha remove out-bfs.pdf out-bfs.gif
    
}*/

int main() {
  VisualTest::testBFS();
}

#include <sealib/_types.h>
#include <sealib/graph/graphcreator.h>
#include "./test_visual.h"
#include "sealibvisual/examples.h"
#include "sealibvisual/tikzarray.h"
#include "sealibvisual/tikzdocument.h"
#include "sealibvisual/tikzfigure.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealibvisual/tikzgraph.h"
#include "sealibvisual/tikznode.h"
#include "sealibvisual/tikzpicture.h"
#include "sealibvisual/tikzstyle.h"

void tikz_example() {
    unsigned int order = 4;
    auto **adj_mtrx = (unsigned int **)malloc(sizeof(unsigned int) * 4 * 4);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};

    Sealib::UndirectedGraph bg =
        Sealib::GraphCreator::createFromAdjacencyMatrix(adj_mtrx, order);
    std::shared_ptr<SealibVisual::TikzGraph> vg =
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

    for (auto &e : vg->getEdges()) {
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

int main() { SealibVisual::VisualTest::testBFS(); }

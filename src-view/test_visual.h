#ifndef SRC_VIEW_TEST_VISUAL_H_
#define SRC_VIEW_TEST_VISUAL_H_
#include <vector>
#include "sealib/graph/graphcreator.h"
#include "sealib/iterator/edgemarker.h"
#include "sealibvisual/examples.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealib/graph/graphio.h"

namespace SealibVisual {

class VisualTest {
 public:
    static void testBFS() {
        uint n = 20;
        Sealib::DirectedGraph g =
            Sealib::GraphCreator::createRandomKRegularGraph(n, 3);
        VisualBFS b(&g, Sealib::CompactArray(n, 4), "out-bfs.tex");
        b.run();
    }

    static void testDFS() {
        uint n = 50;
        Sealib::DirectedGraph g =
            Sealib::GraphCreator::createRandomKRegularGraph(n, 1);
        //Sealib::GraphExporter::exportGML(&g,true,"dfs.gml");
        //Sealib::DirectedGraph g=Sealib::GraphImporter::importGML<Sealib::DirectedGraph>("dfs.gml");
        Sealib::CompactArray c(n, 3);
        VisualDFS d(&g, &c, "out-dfs.tex", "beamer");
        d.run();
    }

    static void testCutVertex() {
        uint n = 20;
        Sealib::UndirectedGraph *g =
            Sealib::GraphCreator::createRandomUndirected(n, 2).first;
        TikzDocument doc("out-cutvertex.tex",
                         "matrix,graphdrawing,positioning,quotes",
                         "layered,force", true);
        std::shared_ptr<TikzPicture> pic(new TikzPicture(
            "spring electrical layout, sibling distance=15mm, node "
            "distance=20mm, node sep=1cm, arrows={->}, line "
            "width=1pt, color=black"));
        std::shared_ptr<TikzGraph> tg = TikzGenerator::generateTikzElement(g);
        pic->add(tg);
        std::shared_ptr<VisualEdgeMarker> e(
            new VisualEdgeMarker(g, &doc, pic, tg));
        VisualCutVertex c(e);
        c.init();
        while (c.more()) c.next();
        doc.close();
    }

    static void testBitset() {
        std::vector<std::string> numbers1(10);
        for (unsigned int i = 0; i < 10; i++) {
            numbers1[i] = std::to_string(i * 45);
        }

        Sealib::Bitset<unsigned char> bits(10);
        bits[4] = 1;

        std::shared_ptr<TikzElement> tikzNode =
            TikzGenerator::generateTikzElement(bits);

        TikzDocument doc("out.tex", "matrix, backgrounds");
        doc.add(tikzNode);
        doc.close();
    }
};

}  // namespace SealibVisual
#endif  // SRC_VIEW_TEST_VISUAL_H_

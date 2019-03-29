#ifndef SRC_VIEW_TEST_VISUAL_H_
#define SRC_VIEW_TEST_VISUAL_H_
#include <vector>
#include "sealib/graph/graphcreator.h"
#include "sealib/graph/graphio.h"
#include "sealib/iterator/edgemarker.h"
#include "sealibvisual/examples.h"
#include "sealibvisual/tikzgenerator.h"

namespace SealibVisual {

class VisualTest {
 public:
    static void testBFS() {
        uint64_t n = 20;
        Sealib::DirectedGraph g =
            Sealib::GraphImporter::importGML<Sealib::DirectedGraph>(
                "graph-bfs-v1.gml");
        VisualBFS b(g, Sealib::CompactArray(n, 4), "demo-bfs.tex", "beamer");
        b.run();
    }

    static void testDFS() {
        uint64_t n = 50;
        Sealib::DirectedGraph g =
            Sealib::GraphCreator::kOutdegree(n, 1);
        Sealib::CompactArray c(n, 3);
        VisualDFS d(g, &c, "out-dfs.tex", "beamer");
        d.run();
    }

    static void testCutVertex() {
        Sealib::UndirectedGraph g = Sealib::GraphCreator::windmill(3, 4);
        std::shared_ptr<VisualEdgeMarker> e(
            new VisualEdgeMarker(g, "out-cutvertex.tex", "beamer"));
        e->init();
        VisualCutVertex c(e);
        c.init();
        while (c.more()) c.next();
    }

    static void testBCC() {
        Sealib::UndirectedGraph g =
            Sealib::GraphCreator::kRegular(20, 2);
        std::shared_ptr<VisualEdgeMarker> e(
            new VisualEdgeMarker(g, "out-bcc.tex", "beamer", true));
        e->init();
        VisualBCC b(e);
        b.init(42);
        while (b.more()) b.next();
    }

    static void testBitset() {
        std::vector<std::string> numbers1(10);
        for (uint64_t i = 0; i < 10; i++) {
            numbers1[i] = std::to_string(i * 45);
        }

        Sealib::Bitset<uint8_t> bits(10);
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

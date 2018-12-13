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
        Sealib::UndirectedGraph g =
            Sealib::GraphCreator::createWindmill(3,4);
        std::shared_ptr<VisualEdgeMarker> e(new VisualEdgeMarker(&g,"out-cutvertex.tex","beamer"));
        e->init();
        VisualCutVertex c(e);
        c.init();
        while (c.more()) c.next();
    }

    static void testBCC() {
        //Sealib::UndirectedGraph g=Sealib::GraphCreator::createRandomKRegularUndirectedGraph(20,2);
        //Sealib::UndirectedGraph g=Sealib::GraphCreator::createWindmill(3,4);
        uint size = 10;
        Sealib::UndirectedGraph g(size);
        for (uint a = 0; a < size - 1; a++) {
            uint i1 = g.deg(a), i2 = g.deg(a + 1);
            g.getNode(a).addAdjacency({a + 1, i2});
            g.getNode(a + 1).addAdjacency({a, i1});
        }
        std::shared_ptr<VisualEdgeMarker> e(new VisualEdgeMarker(&g,"out-bcc.tex","standalone",true));
        e->init();
        VisualBCC b(e);
        b.init();
        b.start(4,g.head(4,0));
        while (b.more()) b.next();
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

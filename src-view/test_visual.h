#include <gtest/gtest.h>
#include <vector>
#include "sealibvisual/examples.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealib/graph/graphcreator.h"

namespace SealibVisual {

class VisualTest {
 public:
    static void testBFS() {
        uint n = 20;
        Sealib::DirectedGraph g =
            Sealib::GraphCreator::createRandomKRegularGraph(n, 3);
        VisualBFS b(&g, Sealib::CompactArray(n, 4), "out-bfs3.tex");
        b.run();
    }

    static void testDFS() {
        uint n = 20;
        Sealib::DirectedGraph g =
            Sealib::GraphCreator::createRandomKRegularGraph(n, 3);
        Sealib::CompactArray c(n, 3);
        VisualDFS d(&g, &c, "out-dfs3.tex");
        d.run();
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

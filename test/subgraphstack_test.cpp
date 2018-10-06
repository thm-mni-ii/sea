#include <gtest/gtest.h>
#include <sealib/subgraphstack.h>
#include <include/sealib/graphcreator.h>

using Sealib::SubGraphStack;
using Sealib::SubGraph;
using Sealib::RecursiveSubGraph;
using Sealib::BaseSubGraph;
using Sealib::Bitset;
using Sealib::BasicGraph;
using Sealib::GraphCreator;
using std::shared_ptr;

TEST(SubGraphStackTest, nonZeroDegree) {
    typedef Sealib::Bitset<unsigned char> bitset_t;

    unsigned int order = 7;
    auto **adj_mtrx = (unsigned int **) malloc(sizeof(unsigned int) * 7 * 7);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0, 0, 1, 1};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 0, 0, 0, 1};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 0, 0, 1, 1, 0};
    adj_mtrx[4] = new unsigned int[order]{0, 0, 0, 1, 0, 1, 0};
    adj_mtrx[5] = new unsigned int[order]{1, 1, 0, 1, 1, 0, 0};
    adj_mtrx[6] = new unsigned int[order]{1, 1, 1, 0, 0, 0, 0};

    shared_ptr<BasicGraph> bg =
        GraphCreator::createSharedGraphFromAdjacencyMatrix(adj_mtrx, order);

    SubGraphStack stack(bg);

    for (unsigned long i = 0; i < 4; i++) {
        bitset_t a(stack.gMax(i));
        for (unsigned long j = 0; j < a.blocks(); j++) {
            a.setBlock(j, (unsigned char) - 1);
        }
        for(unsigned long j = 0; j < a.size(); j++) {
            if(j % 4 == 0) {
                std::tuple<unsigned long, unsigned long> gInv = stack.gInv(j+1);
                std::tuple<unsigned long, unsigned long> mate
                    = stack.mate(std::get<0>(gInv), std::get<1>(gInv));
                unsigned long mateArc = stack.g(std::get<0>(mate), std::get<1>(mate));
                a[j] = 0;
                a[mateArc-1] = 0;
            }
        }
        stack.push(a);
    }
}

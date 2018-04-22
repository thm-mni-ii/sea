#include <iostream>
#include <sealib/graphcreator.h>
#include <sealib/graphalgorithms.h>

using namespace std;

int main() {
    unsigned int order = 18;
    auto **adj_mtrx = new unsigned int *[order];

    adj_mtrx[0] = new unsigned int[order]   {0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[2] = new unsigned int[order]   {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[3] = new unsigned int[order]   {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[4] = new unsigned int[order]   {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[5] = new unsigned int[order]   {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[6] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[7] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[8] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[9] = new unsigned int[order]   {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[10] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    adj_mtrx[11] = new unsigned int[order]  {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0};

    adj_mtrx[12] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[13] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 5, 0, 0};
    adj_mtrx[14] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    adj_mtrx[15] = new unsigned int[order]  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0};

    adj_mtrx[16] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    adj_mtrx[17] = new unsigned int[order]  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Graph *g  = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
    GraphAlgorithms::dotFileFromGraph(g, "graph.dot");
    TrailStructure **ts = GraphAlgorithms::hierholzer(g);
    cout << GraphAlgorithms::stringFromTrail(g, ts);

    GraphAlgorithms::dotFileFromTrail(g, ts, "trail.dot");
}
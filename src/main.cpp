#include <iostream>
#include <sealib/graphcreator.h>
#include <sealib/graphalgorithms.h>

using namespace std;

int main() {
    unsigned int order = 5;
    unsigned int **adj_mtrx = new unsigned int *[order];
    /**
     * (n)       0       1       2       3
     *      **********************************
     *  0   *    0   *   2   *   0   *   1   *
     *      **********************************
     *  1   *    2   *   0   *   1   *   0   *
     *      **********************************
     *  2   *    0   *   1   *   0   *   1   *
     *      **********************************
     *  3   *    1   *   0   *   1   *   0   *
     *      **********************************
     */
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0, 1};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1, 0};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0, 0};
    adj_mtrx[4] = new unsigned int[order]{1, 1, 0, 0, 0};

    Graph *g = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);

    TrailStructure **ts = GraphAlgorithms::hierholzer(g);
}
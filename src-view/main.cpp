#include <iostream>
#include <chrono>
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"

using std::cout;
using Sealib::DFS;

int main() {
    // Graph in the swapped begin pointer representation
    auto *graph = new unsigned int[19]{ 5, 9, 7, 9, 9, 7, 12, 1, 17, 2, 12, 14, 3, 14, 4, 12, 17, 5, 14 };
    auto *graph3 = new unsigned int[19]{ /*n*/ 3, 7, 5, 5, /*m*/ 6, 1, 9, 2, 9, 3, 7 };
    auto *graph4 = new unsigned int[10]{3, 7,8,5, 5, 1,9, 2, 3,7};
    auto *graph5 = new unsigned int[15]{5, 9,10,7,13,7, 8, 1,12, 2, 3,9, 4, 5,12};

    // Grade zero
    auto *graph0 = new unsigned int[9]{4,2,2,3,4,3,1,3,4};

    auto wcts = std::chrono::system_clock::now();
    DFS::runLinearTimeInplaceDFS(graph0, 1);
    std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);
    std::cout << "Finished in " << wctduration.count() << " seconds [Wall Clock]" << std::endl;

//    unsigned int order = 4;
//    unsigned int **adj_mtrx = new unsigned int *[order];
//    /**
//     * (n)       0       1       2       3
//     *      **********************************
//     *  0   *    0   *   2   *   0   *   1   *
//     *      **********************************
//     *  1   *    2   *   0   *   1   *   0   *
//     *      **********************************
//     *  2   *    0   *   1   *   0   *   1   *
//     *      **********************************
//     *  3   *    1   *   0   *   1   *   0   *
//     *      **********************************
//     */
//    adj_mtrx[0] = new unsigned int[order]{0, 2, 0, 1};
//    adj_mtrx[1] = new unsigned int[order]{2, 0, 1, 0};
//    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
//    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};
//
//    Graph *g = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
//
//    for (unsigned int i = 0; i < g->getOrder(); i++) {
//        Node *n = g->getNode(i);
//        Adjacency *a = n->getAdj();
//        cout << "Node(" << i << "): ";
//        for (unsigned int j = 0; j < n->getDegree(); j++) {
//            cout << "(" << a[j].vertex << ", " << a[j].crossIndex << ") ";
//        }
//        cout << "\n";
//    }
}

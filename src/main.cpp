
#include <iostream>
#include <sealib/graphcreator.h>

using namespace std;

int main() {
    unsigned int order = 4;
    unsigned int **adj_mtrx = (unsigned int**) std::malloc(sizeof(unsigned int) * 4 * 4);
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
    adj_mtrx[0] = new unsigned int[order]{0,2,0,1};
    adj_mtrx[1] = new unsigned int[order]{2,0,1,0};
    adj_mtrx[2] = new unsigned int[order]{0,1,0,1};
    adj_mtrx[3] = new unsigned int[order]{1,0,1,0};

    Graph *g = GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);

    for(unsigned int i = 0; i < g->getOrder(); i++) {
        Node *n = g->getNode(i);
        Adjacency* a = n->getAdj();
        cout << "Node(" << i << "): ";
        for(unsigned int j = 0; j < n->getDeg(); j++) {
            cout << "(" << a[j].vertex << ", " << a[j].crossIndex << ") ";
        }
        cout << "\n";
    }
}
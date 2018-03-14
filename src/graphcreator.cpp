//
// Created by jmeintrup on 14.03.18.
//

#include <malloc.h>
#include "sealib/graphcreator.h"

/**
 * Static function to create a a graph object from an adjacency matrix.
 *
 * example matrix with order 3:
 *
 * n         0       1       2
 *      **************************
 *  0   *    0   *   1   *   1   *
 *      **************************
 *  1   *    1   *   0   *   2   *
 *      **************************
 *  2   *    0   *   2   *   1   *
 *      **************************
 * @param adj_matrix NxN adjcacency matrix representation of the graph.
 * @param _order Order of the graph, order equals the number of nodes.
 */
Graph *GraphCreator::createGraphFromAdjacencyMatrix(unsigned int **adjMatrix, unsigned int order) {
    Node *nodes = (Node*) malloc(sizeof(Node) * order);

    for(unsigned int i = 0; i < order; i ++) {
        unsigned int deg = 0;

        for(unsigned int j = 0; j < order; j++) {
            deg += adjMatrix[i][j];
        }

        auto *adj = (Adjacency*) malloc(sizeof(Adjacency) * deg);

        int idx = 0;
        for(unsigned int j = 0; j < order; j++) {
            for(unsigned int k = 0; k < adjMatrix[i][j]; k++) {
                adj[idx++] = Adjacency(j);
            }
        }
        nodes[i] = Node(adj, deg);
    }

    return new Graph(nodes, order);
}
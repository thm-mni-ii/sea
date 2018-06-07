#include <cstdlib>
#include <limits>
#include "sealib/graphcreator.h"

void Sealib::GraphCreator::createGraphFromAdjacencyMatrix(const std::vector<std::vector<unsigned int>>& adjMatrix, unsigned int order, Sealib::Graph& graph) {
    std::vector<Sealib::Node> nodes(order);
    for (unsigned int i = 0; i < order; i++) {
        unsigned int deg = 0;

        for (unsigned int j = 0; j < order; j++) {
            deg += adjMatrix[i][j];
        }

        std::vector<Sealib::Adjacency> adj(deg);
        int idx = 0;
        for (unsigned int j = 0; j < order; j++) {
            for (unsigned int k = 0; k < adjMatrix[i][j]; k++) {
                adj[idx++] = Sealib::Adjacency(j);
            }
        }
        nodes[i].setAdjacencies(adj);
    }

    for (unsigned int i = 0; i < order; i++) {
        const unsigned int deg = nodes[i].getDegree();
        std::vector<Sealib::Adjacency>& adj_arr = nodes[i].getAdj();

        for (unsigned int j = 0; j < deg; j++) {
            if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
                unsigned int v = adj_arr[j].vertex;
                std::vector<Sealib::Adjacency>& _adj_arr = nodes[v].getAdj();
                const unsigned int _deg = nodes[v].getDegree();

                for (unsigned int _j = 0; _j < _deg; _j++) {
                    if (_adj_arr[_j].crossIndex == std::numeric_limits<unsigned int>::max() &&
                        _adj_arr[_j].vertex == i) {
                        _adj_arr[_j].crossIndex = j;
                        adj_arr[j].crossIndex = _j;
                        break;
                    }
                }
            }
        }
    }
    graph.setNodes(nodes);
}


void Sealib::GraphCreator::randomEulerianOgdfGraph(unsigned int nodeMax, unsigned int edgeMax, Sealib::Graph &g) {
    std::vector<std::vector<unsigned int>> adjMatrix(static_cast<unsigned long>(nodeMax), std::vector<unsigned int>(static_cast<unsigned long>(nodeMax)));
    unsigned int edgeCount = 0;
    std::vector<unsigned int> uneven;
    std::vector<unsigned int> even;

    uneven.push_back(0);
    uneven.push_back(nodeMax - 1);

    for (unsigned int i = 1; i < nodeMax - 1; i++) {
        even.push_back(i);
    }

    for (unsigned int i = 0; i < nodeMax - 1; i++) {
        adjMatrix[i][i+1]++;
        adjMatrix[i+1][i]++;
        edgeCount++;
    }

    while (edgeCount < edgeMax) {
        int nextEven;
        auto r1 = static_cast<int>(rand() % (even.size() - 1));

        nextEven = even.at(r1);

        int nextUneven;
        auto r2 = static_cast<int>(rand() % (uneven.size() - 1));

        nextUneven = uneven.at(r2);

        adjMatrix[nextEven][nextUneven]++;
        adjMatrix[nextUneven][nextEven]++;
        edgeCount++;

        even.at(r1) = nextUneven;
        uneven.at(r2) = nextEven;
    }
    createGraphFromAdjacencyMatrix(adjMatrix, nodeMax, g);
}
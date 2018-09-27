#include "sealib/graphcreator.h"
#include <stdlib.h>
#include <limits>

using Sealib::BasicGraph;
using Sealib::GraphCreator;

BasicGraph *GraphCreator::createGraphFromAdjacencyMatrix(
    unsigned int **adjMatrix, unsigned int order) {
  Node *nodes = static_cast<Node *>(malloc(sizeof(Node) * order));

  for (unsigned int i = 0; i < order; i++) {
    unsigned int deg = 0;

    for (unsigned int j = 0; j < order; j++) {
      deg += adjMatrix[i][j];
    }

    Adjacency *adj = static_cast<Adjacency *>(malloc(sizeof(Adjacency) * deg));

    int idx = 0;
    for (unsigned int j = 0; j < order; j++) {
      for (unsigned int k = 0; k < adjMatrix[i][j]; k++) {
        adj[idx++] = Adjacency(j);
      }
    }
    nodes[i] = Node(adj, deg);
  }

  for (unsigned int i = 0; i < order; i++) {
    const unsigned int deg = nodes[i].getDegree();
    Adjacency *adj_arr = nodes[i].getAdj();

    for (unsigned int j = 0; j < deg; j++) {
      if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
        unsigned int v = adj_arr[j].vertex;
        Adjacency *_adj_arr = nodes[v].getAdj();
        const unsigned int _deg = nodes[v].getDegree();

        for (unsigned int _j = 0; _j < _deg; _j++) {
          if (_adj_arr[_j].crossIndex ==
                  std::numeric_limits<unsigned int>::max() &&
              _adj_arr[_j].vertex == i) {
            _adj_arr[_j].crossIndex = j;
            adj_arr[j].crossIndex = _j;
            break;
          }
        }
      }
    }
  }

  return new BasicGraph(nodes, order);
}

static std::random_device rng;

BasicGraph *GraphCreator::createRandomFixed(unsigned int order,
                                            unsigned int degreePerNode) {
  std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
  Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
  for (unsigned int a = 0; a < order; a++) {
    unsigned int ai = degreePerNode;
    Adjacency *ad =
        reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * ai));
    for (unsigned int b = 0; b < ai; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad, ai);
  }
  return new BasicGraph(n, order);
}

BasicGraph *GraphCreator::createRandomGenerated(unsigned int order) {
  Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
  std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
  for (unsigned int a = 0; a < order; a++) {
    unsigned int deg = rnd(rng);
    Adjacency *ad =
        reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * deg));
    for (unsigned int b = 0; b < deg; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad, deg);
  }
  return new BasicGraph(n, order);
}

#include "sealib/graphcreator.h"
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <set>

Sealib::BasicGraph Sealib::GraphCreator::createGraphFromAdjacencyMatrix(
    unsigned int **adjMatrix, unsigned int order) {
  std::vector<Node> nodes;

  for (unsigned int i = 0; i < order; i++) {
    unsigned int deg = 0;

    for (unsigned int j = 0; j < order; j++) {
      deg += adjMatrix[i][j];
    }

    std::vector<Adjacency> adj(deg);

    int idx = 0;
    for (unsigned int j = 0; j < order; j++) {
      for (unsigned int k = 0; k < adjMatrix[i][j]; k++) {
        adj[idx++] = Adjacency(j);
      }
    }
    nodes.emplace_back(adj);
  }
  for (unsigned int i = 0; i < order; i++) {
    const unsigned int deg = nodes[i].getDegree();
    const std::vector<Adjacency> &adj_arr = nodes[i].getAdj();

    for (unsigned int j = 0; j < deg; j++) {
      if (adj_arr[j].crossIndex == std::numeric_limits<unsigned int>::max()) {
        unsigned int v = adj_arr[j].vertex;
        const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
        const unsigned int _deg = nodes[v].getDegree();

        for (unsigned int _j = 0; _j < _deg; _j++) {
          if (_adj_arr[_j].crossIndex ==
                  std::numeric_limits<unsigned int>::max() &&
              _adj_arr[_j].vertex == i) {
            nodes[v].setCrossIndex(_j, j);
            nodes[i].setCrossIndex(j, _j);
            break;
          }
        }
      }
    }
  }

  return BasicGraph(nodes);
}

std::unique_ptr<Sealib::BasicGraph>
Sealib::GraphCreator::generateRandomBasicGraph(unsigned int order, double p,
                                               unsigned int seed) {
  std::unique_ptr<Sealib::BasicGraph> graph(new Sealib::BasicGraph(order));

  std::mt19937_64 rng(seed);
  std::uniform_real_distribution<double> unif(0.0, 1.0);

  for (unsigned int n1 = 0; n1 < order - 1; n1++) {
    for (unsigned int n2 = 1; n2 < order; n2++) {
      if (n1 != n2) {
        bool hasEdge = false;
        for (const auto &adj : graph->getNode(n1).getAdj()) {
          if (adj.vertex == n2) {
            hasEdge = true;
            break;
          }
        }
        if (!hasEdge && unif(rng) < p) {
          Sealib::Node &node1 = graph->getNode(n1);
          Sealib::Node &node2 = graph->getNode(n2);

          unsigned int n1idx = node1.getDegree();
          unsigned int n2idx = node2.getDegree();

          node1.addAdjacency(n2);
          node1.setCrossIndex(n1idx, n2idx);

          node2.addAdjacency(n1);
          node2.setCrossIndex(n2idx, n1idx);
        }
      }
    }
  }

  return graph;
}

std::unique_ptr<Sealib::BasicGraph>
Sealib::GraphCreator::generateRandomBipartiteBasicGraph(unsigned int order1,
                                                        unsigned int order2,
                                                        double p,
                                                        unsigned int seed) {
  std::unique_ptr<Sealib::BasicGraph> graph(
      new Sealib::BasicGraph(order1 + order2));

  std::mt19937_64 rng(seed);
  std::uniform_real_distribution<double> unif(0.0, 1.0);

  for (unsigned int n1 = 0; n1 < order1; n1++) {
    for (unsigned int n2 = order1; n2 < order2; n2++) {
      if (unif(rng) < p) {
        Sealib::Node &node1 = graph->getNode(n1);
        Sealib::Node &node2 = graph->getNode(n2);

        unsigned int n1idx = node1.getDegree();
        unsigned int n2idx = node2.getDegree();

        node1.addAdjacency(n2);
        node1.setCrossIndex(n1idx, n2idx);

        node2.addAdjacency(n1);
        node2.setCrossIndex(n2idx, n1idx);
      }
    }
  }

  return graph;
}

static std::random_device rng;

Sealib::BasicGraph *Sealib::GraphCreator::createRandomFixed(
    unsigned int order, unsigned int degreePerNode) {
  std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
  std::vector<Node> n(order);
  for (unsigned int a = 0; a < order; a++) {
    unsigned int ai = degreePerNode;
    std::vector<Adjacency> ad(ai);
    for (unsigned int b = 0; b < ai; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad);
  }
  return new BasicGraph(n);
}

Sealib::BasicGraph *Sealib::GraphCreator::createRandomImbalanced(
    unsigned int order) {
  Node *n = reinterpret_cast<Node *>(malloc(sizeof(Node) * order));
  std::uniform_int_distribution<unsigned int> rnd(0, order - 1);
  std::uniform_int_distribution<unsigned int> dist1(order * order,
                                                    2 * order * order);
  std::uniform_int_distribution<unsigned int> dist2(
      0, static_cast<unsigned int>(ceil(log2(order))));
  std::set<unsigned> big;
  for (unsigned a = 0; a < ceil(order / (2 * log2(order))); a++)
    big.insert(rnd(rng));
  for (unsigned int a = 0; a < order; a++) {
    unsigned int deg;
    if (big.find(a) == big.end()) {
      deg = dist2(rng);
    } else {
      deg = dist1(rng);
    }
    Adjacency *ad =
        reinterpret_cast<Adjacency *>(malloc(sizeof(Adjacency) * deg));
    for (unsigned int b = 0; b < deg; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad, deg);
  }
  return new Sealib::BasicGraph(n, order);
}

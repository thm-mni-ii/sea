#include <sealib/graphcreator.h>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <set>

using Sealib::BasicGraph;
using Sealib::GraphCreator;

BasicGraph *Sealib::GraphCreator::createGraphPointerFromAdjacencyMatrix(
    uint32_t **adjMatrix, uint32_t order) {
  std::vector<Node> nodes(order);

  for (uint32_t i = 0; i < order; i++) {
    uint32_t deg = 0;

    for (uint32_t j = 0; j < order; j++) {
      deg += adjMatrix[i][j];
    }

    std::vector<Adjacency> adj(deg);

    uint32_t idx = 0;
    for (uint32_t j = 0; j < order; j++) {
      for (uint32_t k = 0; k < adjMatrix[i][j]; k++) {
        adj[idx++] = Adjacency(j);
      }
    }
    nodes[i] = Node(adj);
  }
  for (uint32_t i = 0; i < order; i++) {
    const uint32_t deg = nodes[i].getDegree();
    const std::vector<Adjacency> &adj_arr = nodes[i].getAdj();

    for (uint32_t j = 0; j < deg; j++) {
      if (adj_arr[j].crossIndex == std::numeric_limits<uint32_t>::max()) {
        uint32_t v = adj_arr[j].vertex;
        const std::vector<Adjacency> &_adj_arr = nodes[v].getAdj();
        const uint32_t _deg = nodes[v].getDegree();

        for (uint32_t _j = 0; _j < _deg; _j++) {
          if (_adj_arr[_j].crossIndex == std::numeric_limits<uint32_t>::max() &&
              _adj_arr[_j].vertex == i) {
            nodes[v].setCrossIndex(_j, j);
            nodes[i].setCrossIndex(j, _j);
            break;
          }
        }
      }
    }
  }
  return new BasicGraph(nodes);
}

BasicGraph GraphCreator::createGraphFromAdjacencyMatrix(uint32_t **adjMatrix,
                                                        uint32_t order) {
  return std::move(*createGraphPointerFromAdjacencyMatrix(adjMatrix, order));
}

std::shared_ptr<BasicGraph> GraphCreator::createSharedGraphFromAdjacencyMatrix(
    uint32_t **adjMatrix, uint32_t order) {
  return std::shared_ptr<BasicGraph>(
    createGraphPointerFromAdjacencyMatrix(adjMatrix, order));
}

std::unique_ptr<Sealib::BasicGraph>
Sealib::GraphCreator::generateRandomBipartiteBasicGraph(uint32_t order1,
                                                        uint32_t order2,
                                                        double p,
                                                        uint32_t seed) {
  std::unique_ptr<Sealib::BasicGraph> graph(
      new Sealib::BasicGraph(order1 + order2));

  std::mt19937_64 _rng(seed);
  std::uniform_real_distribution<double> unif(0.0, 1.0);

  for (uint32_t n1 = 0; n1 < order1; n1++) {
    for (uint32_t n2 = order1; n2 < order2; n2++) {
      if (unif(_rng) < p) {
        Sealib::Node &node1 = graph->getNode(n1);
        Sealib::Node &node2 = graph->getNode(n2);

        uint32_t n1idx = node1.getDegree();
        uint32_t n2idx = node2.getDegree();

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

BasicGraph GraphCreator::createRandomImbalanced(uint32_t order) {
  std::vector<Node> n(order);
  std::uniform_int_distribution<uint32_t> rnd(0, order - 1);
  std::uniform_int_distribution<uint32_t> dist1(order * order,
                                                2 * order * order);
  std::uniform_int_distribution<uint32_t> dist2(
      0, static_cast<uint32_t>(ceil(log2(order))));
  std::set<unsigned> big;
  for (uint32_t a = 0; a < ceil(order / (2 * log2(order))); a++)
    big.insert(rnd(rng));
  for (uint32_t a = 0; a < order; a++) {
    uint32_t deg;
    if (big.find(a) == big.end()) {
      deg = dist2(rng);
    } else {
      deg = dist1(rng);
    }
    std::vector<Adjacency> ad(deg);
    for (uint32_t b = 0; b < deg; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad);
  }
  return std::move(BasicGraph(n));
}

Sealib::BasicGraph Sealib::GraphCreator::createRandomFixed(
    uint32_t order, uint32_t degreePerNode) {
  std::uniform_int_distribution<uint32_t> rnd(0, order - 1);
  std::vector<Node> n(order);
  for (uint32_t a = 0; a < order; a++) {
    uint32_t ai = degreePerNode;
    std::vector<Adjacency> ad(ai);
    for (uint32_t b = 0; b < ai; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad);
  }
  return std::move(BasicGraph(n));
}

Sealib::BasicGraph Sealib::GraphCreator::createRandomGenerated(uint32_t order) {
  std::vector<Node> n(order);
  std::uniform_int_distribution<uint32_t> rnd(0, order - 1);
  for (uint32_t a = 0; a < order; a++) {
    uint32_t deg = rnd(rng);
    std::vector<Adjacency> ad(deg);
    for (uint32_t b = 0; b < deg; b++) {
      ad[b] = Adjacency(rnd(rng));
    }
    n[a] = Node(ad);
  }
  return std::move(BasicGraph(n));
}

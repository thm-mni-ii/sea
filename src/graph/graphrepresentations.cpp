#include "sealib/graphrepresentations.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <set>
#include "sealib/adjacency.h"
#include "sealib/compactgraph.h"
#include "sealib/node.h"

using Sealib::Graphrepresentations;
using Sealib::Compactgraph;

uint* Graphrepresentations::fastGraphGeneration(uint n, uint mPern) {
  uint M = n * mPern;
  uint N = n + M + 2;

  uint32_t *A = new uint32_t[N];
  A[0] = n;
  A[n + 1] = M;
  std::random_device rd;
  std::mt19937_64 mt(rd());
  mt.seed(std::mt19937_64::default_seed);

  A[1] = n + 2;

  for (uint v = 1; v <= n; ++v) {
    // First: Create Pointer!
    if (v != n) {
      A[v + 1] = A[v] + mPern;
    }
    std::set<uint> generated;
    for (uint p = A[v]; p < N && (v == n || p < A[v + 1]); p++) {
      std::uniform_int_distribution<uint> dist(1, n);
      uint rn = dist(mt);
      if (generated.find(rn) != generated.end()) {
        --p;
      } else {
        A[p] = rn;
        generated.insert(rn);
      }
    }
    generated.clear();
    if (v != n) {
      std::sort(A + (A[v]), A + (A[v + 1]));
    }
  }
  std::sort(A + (A[n]), A + N);
  return A;
}

Compactgraph* Graphrepresentations::generateGilbertGraph(uint32_t order,
                                                    double p,
                                                    std::mt19937_64* gen) {
  return new Compactgraph(generateRawGilbertGraph(order, p, gen));
}

uint* Graphrepresentations::generateRawGilbertGraph(uint32_t order,
                                                         double p,
                                                         std::mt19937_64* gen) {
  uint32_t size = 0;
  uint32_t* edgeArray = new uint32_t[order];
  std::binomial_distribution<uint32_t> dist(order - 1, p);
  for (uint32_t i = 0; i < order; ++i) {
    uint32_t edges = dist(*gen);
    dist.reset();
    size += edges;
    edgeArray[i] = edges;
  }
  uint32_t* graph = new uint32_t[order + size + 2];

  graph[0] = order;
  graph[order + 1] = size;

  uint32_t lastPosition = order + 2;
  for (uint32_t i = 1; i <= order; ++i) {
    if (edgeArray[i - 1] == 0) {
      graph[i] = i;
    } else {
      graph[i] = lastPosition;
      lastPosition += edgeArray[i - 1];
    }
  }
  bool initialBit = 0;
  if (p > 0.5) {
    initialBit = 1;
  }
  std::vector<bool> bitVector(order, initialBit);
  for (uint32_t i = 0; i < order; ++i) {
    uint32_t numBitsSet = 0;
    // a[i] = number of edges from i
    uint32_t bitsToSet = edgeArray[i];
    if (initialBit == 1) {
      bitsToSet = order - bitsToSet;
    }
    while (edgeArray[i] > numBitsSet) {
      uint32_t rnd = std::rand() % order;
      if (bitVector[rnd] == initialBit && rnd != i) {
        bitVector[rnd] = !initialBit;
        numBitsSet += 1;
      }
    }
    uint32_t pos = graph[i + 1];
    for (uint32_t j = 0; j < order; ++j) {
      if (bitVector[j] == !initialBit) {
        graph[pos++] = j + 1;
        bitVector[j] = initialBit;
      }
    }
  }
  delete[] edgeArray;
  return graph;
}

/*
uint32_t* Graphrepresentations::graphToStandard(Graph* g) {
  uint32_t order = g->getOrder();
  uint32_t numEdges = 0;
  for (uint32_t i = 0; i < order; ++i) {
    numEdges += g->getNode(i)->getDegree();
  }
  uint32_t* standardgraph = new uint32_t[order + 2 + numEdges];
  standardgraph[0] = order;
  standardgraph[order + 1] = numEdges;
  uint32_t adjptr = order + 2;
  for (uint32_t i = 1; i <= order; ++i) {
    Node* node = g->getNode(i - 1);
    uint32_t degree = node->getDegree();
    Adjacency* adj = node->getAdj();
    standardgraph[i] = adjptr;
    for (uint32_t j = 0; j < degree; ++j) {
      standardgraph[adjptr + j] = adj[j].vertex + 1;
    }
    adjptr += degree;
  }
  return standardgraph;
}

Graph* Graphrepresentations::standardToGraph(uint32_t* a) {
  uint32_t order = a[0];
  uint32_t size = a[order + 1];
  // save the total number of size of the array so we dont have to
  // determine the last vertex in a special case
  a[order + 1] = order + size + 2;
  Node* nodes = static_cast<Node*>(malloc(sizeof(Node) * order));
  for (uint32_t i = 0; i < order; ++i) {
    // vertex names in standard representations start at 1
    uint32_t v = i + 1;
    // if a node points to itself it has no edges
    if (a[v] == v) {
      nodes[i] = Node(nullptr, 0);
    } else {
      // if the neighboring nodes are pointing on themself
      // we have to skip them to determine the degree
      uint32_t pos = 1;
      while (a[v + pos] < order + 1) {
        ++pos;
      }
      uint32_t degree = a[v + pos] - a[v];
      Adjacency* adj =
          static_cast<Adjacency*>(malloc(sizeof(Adjacency) * degree));
      for (uint32_t j = 0; j < degree; ++j) {
        // a[a[v]] points to adj array of v
        adj[j] = Adjacency(a[a[v] + j] - 1);
      }
      nodes[i] = Node(adj, degree);
    }
  }
  return new Graph(nodes, order);
}
*/

void Graphrepresentations::standardToCrosspointer(uint32_t* a) {
  uint32_t n = a[0], v, u, pv, pu;
  // n = order of the graph
  v = u = pv = pu = 0;
  u = 1;
  while (u < n) {
    while (!(a[a[u]] > n || a[u] == a[u + 1])) {
      pu = a[u];
      v = a[pu];
      pv = a[v];
      a[pu] = pv;
      a[pv] = pu;
      ++a[v];
      ++a[u];
    }
    u = u + 1;
  }
  // restore T pointers
  v = n;
  while (v > 1) {
    a[v] = a[v - 1];
    --v;
  }
  a[1] = n + 2;
  return;
}

void Graphrepresentations::standardToBeginpointer(uint32_t* a) {
  uint32_t order = a[0];
  uint32_t numEdges = a[order + 1];
  uint32_t graphSize = order + numEdges + 2;
  for (uint32_t i = order + 2; i < graphSize; ++i) {
    // checks if a[i] is not a node of order 0
    if (a[a[i]] != a[a[i] - 1] || i == order + 2) {
      a[i] = a[a[i]];
    }
  }
  return;
}

void Graphrepresentations::swappedBeginpointerToStandard(uint32_t* a) {
  uint32_t order = a[0];
  uint32_t numEdges = a[order + 1];
  uint32_t graphSize = order + numEdges + 2;
  for (uint32_t i = order + 2; i < graphSize; ++i) {
    if (a[i] > order) {
      a[i] = a[a[i]];
    }
  }
  for (uint32_t i = 1; i <= order; ++i) {
    a[i] = a[a[i]];
  }

  uint32_t v = order;
  while (a[v] == v) {
    --v;
  }
  for (uint32_t i = graphSize - 1; i > order + 1; --i) {
    if (a[i] == v) {
      a[i] = a[v];
      a[v] = i;
      --v;
      while (a[v] == v) {
        --v;
      }
    }
  }
  return;
}

/**
 * Transforms a graph from cross or beginpointer representation to
 * swapped cross or beginpointer representation
 * @param a graph in cross or beginpointer representation
 */
void Graphrepresentations::swapRepresentation(uint32_t* a) {
  uint32_t order = a[0];
  for (uint32_t i = 1; i <= order; ++i) {
    uint32_t temp = a[a[i]];
    a[a[i]] = i;
    a[i] = temp;
  }
  return;
}

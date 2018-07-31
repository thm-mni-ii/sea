#include "sealib/graphrepresentations.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include "sealib/adjacency.h"
#include "sealib/graph.h"
#include "sealib/node.h"

unsigned int* Graphrepresentations::generateGilbertGraph(unsigned int order,
                                                         double p,
                                                         std::mt19937_64* gen) {
  unsigned int size = 0;
  unsigned int* edgeArray = new unsigned int[order];
  std::binomial_distribution<unsigned int> dist(order - 1, p);
  for (unsigned int i = 0; i < order; ++i) {
    unsigned int edges = dist(*gen);
    dist.reset();
    size += edges;
    edgeArray[i] = edges;
  }
  unsigned int* graph = new unsigned int[order + size + 2];

  graph[0] = order;
  graph[order + 1] = size;

  unsigned int lastPosition = order + 2;
  for (unsigned int i = 1; i <= order; ++i) {
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
  for (unsigned int i = 0; i < order; ++i) {
    unsigned int numBitsSet = 0;
    // a[i] = number of edges from i
    unsigned int bitsToSet = edgeArray[i];
    if (initialBit == 1) {
      bitsToSet = order - bitsToSet;
    }
    while (edgeArray[i] > numBitsSet) {
      unsigned int rnd = std::rand() % order;
      if (bitVector[rnd] == initialBit && rnd != i) {
        bitVector[rnd] = !initialBit;
        numBitsSet += 1;
      }
    }
    unsigned int pos = graph[i + 1];
    for (unsigned int j = 0; j < order; ++j) {
      if (bitVector[j] == !initialBit) {
        graph[pos++] = j + 1;
        bitVector[j] = initialBit;
      }
    }
  }
  return graph;
}

unsigned int* Graphrepresentations::graphToStandard(Graph* g) {
  unsigned int order = g->getOrder();
  unsigned int numEdges = 0;
  for (unsigned int i = 0; i < order; ++i) {
    numEdges += g->getNode(i)->getDegree();
  }
  unsigned int* standardgraph = new unsigned int[order + 2 + numEdges];
  standardgraph[0] = order;
  standardgraph[order + 1] = numEdges;
  unsigned int adjptr = order + 2;
  for (unsigned int i = 1; i <= order; ++i) {
    Node* node = g->getNode(i - 1);
    unsigned int degree = node->getDegree();
    Adjacency* adj = node->getAdj();
    standardgraph[i] = adjptr;
    for (unsigned int j = 0; j < degree; ++j) {
      standardgraph[adjptr + j] = adj[j].vertex + 1;
    }
    adjptr += degree;
  }
  return standardgraph;
}

Graph* Graphrepresentations::standardToGraph(unsigned int* a) {
  unsigned int order = a[0];
  unsigned int size = a[order + 1];
  // save the total number of size of the array so we dont have to
  // determine the last vertex in a special case
  a[order + 1] = order + size + 2;
  Node* nodes = static_cast<Node*>(malloc(sizeof(Node) * order));
  for (unsigned int i = 0; i < order; ++i) {
    // vertex names in standard representations start at 1
    unsigned int v = i + 1;
    // if a node points to itself it has no edges
    if (a[v] == v) {
      nodes[i] = Node(nullptr, 0);
    } else {
      // if the neighboring nodes are pointing on themself
      // we have to skip them to determine the degree
      unsigned int pos = 1;
      while (a[v + pos] < order + 1) {
        ++pos;
      }
      unsigned int degree = a[v + pos] - a[v];
      Adjacency* adj =
          static_cast<Adjacency*>(malloc(sizeof(Adjacency) * degree));
      for (unsigned int j = 0; j < degree; ++j) {
        // a[a[v]] points to adj array of v
        adj[j] = Adjacency(a[a[v] + j] - 1);
      }
      nodes[i] = Node(adj, degree);
    }
  }
  return new Graph(nodes, order);
}

void Graphrepresentations::standardToCrosspointer(unsigned int* a) {
  unsigned int n = a[0], v, u, pv, pu;
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

void Graphrepresentations::standardToBeginpointer(unsigned int* a) {
  unsigned int order = a[0];
  unsigned int numEdges = a[order + 1];
  unsigned int graphSize = order + numEdges + 2;
  for (unsigned int i = order + 2; i < graphSize; ++i) {
    // checks if a[i] is not a node of order 0
    if (a[a[i]] != a[a[i] - 1] || i == order + 2) {
      a[i] = a[a[i]];
    }
  }
  return;
}

void Graphrepresentations::swappedBeginpointerToStandard(unsigned int* a) {
  unsigned int order = a[0];
  unsigned int numEdges = a[order + 1];
  unsigned int graphSize = order + numEdges + 2;
  for (unsigned int i = order + 2; i < graphSize; ++i) {
    if (a[i] > order) {
      a[i] = a[a[i]];
    }
  }
  for (unsigned int i = 1; i <= order; ++i) {
    a[i] = a[a[i]];
  }

  unsigned int v = order;
  while (a[v] == v) {
    --v;
  }
  for (unsigned int i = graphSize - 1; i > order + 1; --i) {
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
void Graphrepresentations::swapRepresentation(unsigned int* a) {
  unsigned int order = a[0];
  for (unsigned int i = 1; i <= order; ++i) {
    unsigned int temp = a[a[i]];
    a[a[i]] = i;
    a[i] = temp;
  }
  return;
}

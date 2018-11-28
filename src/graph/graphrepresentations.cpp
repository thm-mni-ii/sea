#include "sealib/graph/graphrepresentations.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <set>
#include "sealib/graph/adjacency.h"
#include "sealib/graph/compactgraph.h"
#include "sealib/graph/node.h"

using Sealib::Graphrepresentations;
using Sealib::CompactGraph;

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
  v = pv = pu = 0;
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

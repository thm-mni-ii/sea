#include "sealib/graphrepresentations.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <set>
#include <bitset>
#include "sealib/adjacency.h"
#include "sealib/compactgraph.h"
#include "sealib/node.h"

using Sealib::Graphrepresentations;
using Sealib::Compactgraph;

uint* Graphrepresentations::fastGraphGeneration(uint n, uint mPern) {
  uint M = n * mPern;
  uint N = n + M + 2;

  unsigned int *A = new unsigned int[N];
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

Compactgraph* Graphrepresentations::generateGilbertGraph(unsigned int order,
                                                    double p,
                                                    std::mt19937_64* gen) {
  return new Compactgraph(generateRawGilbertGraph(order, p, gen));
}

uint* Graphrepresentations::generateRawGilbertGraph(unsigned int order,
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
  delete[] edgeArray;
  return graph;
}

/*
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
*/

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

void Graphrepresentations::standardToShifted(unsigned int *g){
	unsigned int order = g[0];
	unsigned int wordsize = sizeof(*g) * 8;
	unsigned int c = static_cast<unsigned int>(log2(order/wordsize)-1);
	unsigned int c_prime = c + 1;
	unsigned int wordsize_packed = wordsize - c_prime;
  unsigned int c_prime_mask = static_cast<unsigned int>(-1)<<(wordsize-c_prime);	
	unsigned int current_prefix = g[1] & c_prime_mask;
 	std::vector<unsigned int> changing_positions; 
	for(unsigned int i = 1; i < order; ++i){
		if((g[i] & c_prime_mask)!= current_prefix){
				unsigned int num_increments = ((g[i] & c_prime_mask) - current_prefix) >> (wordsize - c_prime);
				current_prefix = g[i] & c_prime_mask; 
				for(unsigned int j = 0; j < num_increments; ++j){
					changing_positions.push_back(i);
				}
		}
	}
	for(unsigned int i = 1; i <= order; ++i){
		unsigned int value = g[i];
		unsigned int startbit = wordsize_packed * (i-1);
		unsigned int offset = startbit & (wordsize-1);
		unsigned int index = (startbit >> (unsigned int)log2(wordsize))+1;
		if(offset + wordsize_packed > wordsize){
			unsigned int leftbits = (wordsize-offset);
			unsigned int rightbits = wordsize_packed - leftbits;
			value = value & ~c_prime_mask;
			g[index] = (value >> rightbits) | g[index];
			g[index+1] = value << (leftbits + c_prime);
		}else{
			g[index] = (g[index] & ~((unsigned int)-1>>offset)) | (value << (c_prime - offset));
		}
	}
}


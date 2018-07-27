//delete later
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "sealib/graphrepresentations.h"
#include "sealib/graph.h"
#include "sealib/node.h"
#include "sealib/adjacency.h"

//	Generates a graph in standard representation with n nodes
//	and ~p(n*(n-1)/2) edges
unsigned int* Graphrepresentations::generateStandardGraph(unsigned int numNodes, float p){
	unsigned int numEdges = 0;
	unsigned int* edgeArray = new unsigned int[numNodes];
	for(unsigned int i = 0; i < numNodes; ++i){
		unsigned int edges = 0;
		for(unsigned int j = 1; j < numNodes; ++j){
			float rnd = static_cast<float>(std::rand())/static_cast<float>(RAND_MAX);
			if(rnd < p){
				++edges;
			}
		}
		numEdges += edges;
		edgeArray[i] = edges;
	}
	unsigned int* graph = new unsigned int[numNodes+numEdges+2];
	graph[0] = numNodes;
	graph[numNodes+1] = numEdges;
	unsigned int lastPosition = numNodes+2;
	if(edgeArray[0] == 0){
		graph[1] = 1;
	}else{
		graph[1] = lastPosition;
		lastPosition += edgeArray[0];
	}

	for(unsigned int i = 2; i <= numNodes; ++i){
		if(edgeArray[i-1] == 0){
			graph[i] = i;
		}else{
			graph[i] = lastPosition;
			lastPosition += edgeArray[i-1];
		}
	}
	bool initialBit = 0;
	if(p > 0.5){
		initialBit = 1;
	}
	std::vector<bool> bitVector(numNodes,initialBit);
	for(unsigned int i = 0; i < numNodes; ++i){
		unsigned int numBitsSet = 0;
		//a[i] = number of edges from i
		unsigned int bitsToSet = edgeArray[i];
		if(initialBit == 1){
			bitsToSet = numNodes - bitsToSet;
		}
		while(edgeArray[i] > numBitsSet){
			unsigned int rnd = std::rand() % numNodes;
			if(bitVector[rnd] == initialBit && rnd != i){
				bitVector[rnd] = !initialBit;
				numBitsSet += 1;
			}
		}
		unsigned int pos = graph[i+1];
		for(unsigned int j = 0; j < numNodes; ++j){
			if(bitVector[j] == !initialBit){
				graph[pos++] = j+1;
				bitVector[j] = initialBit;	
			}
		}
	}
	return graph;
}


//	Transforms graph from graph.h object to standard representation
//	as unsigned int array
unsigned int* Graphrepresentations::graphToStandard(Graph *g){
	unsigned int order = g->getOrder();
	unsigned int numEdges = 0;
	for(unsigned int i = 0; i < order; ++i){
		numEdges += g->getNode(i)->getDegree();
	}
	unsigned int* standardgraph = new unsigned int[order + 2 + numEdges];
	standardgraph[0] = order;
	standardgraph[order + 1] = numEdges;
	unsigned int adjptr = order + 2;
	for(unsigned int i = 1; i <= order; ++i){
		Node *node = g->getNode(i-1);
		unsigned int degree = node->getDegree();
		Adjacency *adj = node->getAdj();
		standardgraph[i] = adjptr;
		for(unsigned int j = 0; j < degree; ++j){
			standardgraph[adjptr + j] = adj[j].vertex + 1;
		}
		adjptr += degree;
	}
	return standardgraph;
}

// Transforms graph inplace from standard representation to crosspointer representation
// TODO: handle graphs with nodes of order 0 and 1
void Graphrepresentations::standardToCrosspointer(unsigned int* a){
	unsigned int n = a[0],v,u,pv,pu;	
	//n = order of the graph
	v = u = pv = pu = 0;
	u = 1; 	
	while(u < n){
		while(!(a[a[u]] > n || a[u] == a[u + 1])){ 
			pu = a[u];
			v = a[pu];
			pv = a[v];
			a[pu] = pv;
			a[pv] = pu;
			++a[v];
			++a[u];
		}
		u = u+1;
	}
	//restore T pointers
	v = n;
	while(v > 1){
		a[v] = a[v-1];
		--v;
	}
	a[1] = n + 2;
	return; 
}

// Transforms graph inplace from standard to beginpointer representation 
void Graphrepresentations::standardToBeginpointer(unsigned int* a){
	unsigned int order = a[0];
	unsigned int numEdges = a[order + 1];
	unsigned int graphSize = order + numEdges + 2;
	for(unsigned int i = order + 2; i < graphSize; ++i){
		//checks if a[i] is not a node of order 0
		if(a[a[i]] != a[a[i]-1] || i == order + 2){
			a[i] = a[a[i]];
		}
	}
	return; 
}

// Transforms graph inplace from beginpointer to standard representation 
void Graphrepresentations::swappedBeginpointerToStandard(unsigned int* a){
	unsigned int order = a[0];
	unsigned int numEdges = a[order + 1];
	unsigned int graphSize = order + numEdges + 2;
	for(unsigned int i = order+2; i < graphSize ; ++i){
		if(a[i] > order){
			a[i] = a[a[i]];
		}
	}
	for(unsigned int i = 1; i <= order; ++i){
		a[i] = a[a[i]];
	}
	
	unsigned int v = order;
	while(a[v] == v){
		--v;
	}
	for(unsigned int i = graphSize - 1; i > order + 1; --i){
		if(a[i] == v){
			a[i] = a[v];
			a[v] = i;
			--v;
			while(a[v] == v){
				--v;
			}
		}
	}
	return; 
}

//Transforms graph inplace from cross or beginpointer to swapped crossor beginpointer representation
void Graphrepresentations::swapRepresentation(unsigned int* a){
	unsigned int order = a[0];
	for(unsigned int i = 1 ; i <= order; ++i){
		unsigned int temp = a[a[i]];
		a[a[i]] = i;
		a[i] = temp;
	}
	return;
}

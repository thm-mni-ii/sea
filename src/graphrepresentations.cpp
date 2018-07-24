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
unsigned int* Graphrepresentations::generateStandardGraph(unsigned int n, float p){
	unsigned int m = 0;
	unsigned int* edgesarray = new unsigned int[n];
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for(unsigned int i = 0; i < n; ++i){
		unsigned int edges = 0;
		for(unsigned int j = 1; j < n; ++j){
			float rnd = static_cast<float>(std::rand())/static_cast<float>(RAND_MAX);
			if(rnd < p){
				++edges;
			}
		}
		//at least 2 egdes
		if(edges < 3){
			edges = 2;
		}
		m += edges;
		edgesarray[i] = edges;
	}
	unsigned int* g = new unsigned int[n+m+2];
	g[0] = n;
	g[n+1] = m;
	//the first node always points one after the stored number of edges
	g[1] = n+2;
	for(unsigned int i = 2; i <= n; ++i){
		g[i] = g[i-1] + edgesarray[i-2];
	}

	std::vector<bool> bitvector(n,0);

	for(unsigned int i = 0; i < n; ++i){
		unsigned int bitsset = 0;
		//a[i] = number of edges from i
		while(edgesarray[i] > bitsset){
			unsigned int rnd = std::rand() % n;
			if(bitvector[rnd] == false && rnd != i){
				bitvector[rnd] = true;
				bitsset += 1;
			}
		}
		unsigned int pos = g[i+1];
		for(unsigned int j = 0; j < n; ++j){
			if(bitvector[j] == true){
				g[pos++] = j+1;
				bitvector[j] = false;	
			}
		}
	}
	return g;
}


//	Transforms graph from graph.h object to standard representation
//	as unsigned int array
unsigned int* Graphrepresentations::graphToStandard(Graph *g){
	unsigned int order = g->getOrder();
	unsigned int numedges = 0;
	for(unsigned int i = 0; i < order; ++i){
		numedges += g->getNode(i)->getDegree();
	}
	unsigned int* standardgraph = new unsigned int[order + 2 + numedges];
	standardgraph[0] = order;
	standardgraph[order + 1] = numedges;
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
	unsigned int numedges = a[order + 1];
	unsigned int asize = order + numedges + 2;
	for(unsigned int i = order + 2; i < asize; ++i){
		a[i] = a[a[i]];
	}
	return; 
}

// Transforms graph inplace from beginpointer to standard representation 
void Graphrepresentations::swappedBeginpointerToStandard(unsigned int* a){
	unsigned int order = a[0];
	unsigned int numedges = a[order + 1];
	unsigned int asize = order + numedges + 2;
	for(unsigned int i = 1; i <= order; ++i){
		if(a[i] > a[0]){
			a[i] = a[a[i]];
		}
	}

	for(unsigned int i = order + 2; i < asize; ++i){
		if(a[i] > a[0]){
			a[i] = a[a[i]];
		}
	}

	unsigned int v = a[0];
	for(unsigned int i = asize-1; i> a[0]+1; --i){
		if(a[i] == v){
			a[i] = a[v];
			a[v]= i;
			--v;
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

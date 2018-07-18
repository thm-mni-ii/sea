#include "sealib/graphrepresentations.h"
#include "sealib/graph.h"
#include "sealib/node.h"
#include "sealib/adjacency.h"
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
unsigned int* Graphrepresentations::standardToCrosspointer(unsigned int* a){
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
	return a;
}

// Transforms graph inplace from standard to beginpointer representation 
unsigned int* Graphrepresentations::standardToBeginpointer(unsigned int* a){
	unsigned int order = a[0];
	unsigned int numedges = a[order + 1];
	unsigned int asize = order + numedges + 2;
	for(unsigned int i = order + 2; i < asize; ++i){
		a[i] = a[a[i]];
	}
	return a;
}

//Transforms graph inplace from cross or beginpointer to swapped crossor beginpointer representation
unsigned int* Graphrepresentations::swapRepresentation(unsigned int* a){
	unsigned int order = a[0];
	for(unsigned int i = 1 ; i <= order; ++i){
		unsigned int temp = a[a[i]];
	  a[a[i]] = i;
		a[i] = temp;
	}
	return a;
}


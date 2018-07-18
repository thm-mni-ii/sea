#include "sealib/graphrepresentations.h"
#include "sealib/graph.h"
#include "sealib/node.h"
#include "sealib/adjacency.h"
//	Transforms Graph from graph.h object to Standard representation
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



#include <stdio.h>
#include "sealib/dfs.h"

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void DFS::process_standard(Graph *g,
						   UserFunc1 preProcess, UserFunc2 preExplore,
						   UserFunc2 postExplore, UserFunc1 postProcess,
						   uint *color, uint u) {
	color[u] = DFS_GRAY;
	Node *un = g->getNode(u);
	if (preProcess != DFS_NOP_PROCESS) preProcess(un);
	for (uint k = 0; k < un->getDegree(); k++) {
		uint v = g->head(u, k);
		Node *vn = g->getNode(v);
		if (preExplore != DFS_NOP_EXPLORE) {
			preExplore(un, vn);
		}
		if (color[v] == DFS_WHITE) {
			process_standard(g, preProcess, preExplore, postExplore, postProcess, color, v);
		}
		if (postExplore != DFS_NOP_EXPLORE) {
			postExplore(un, vn);
		}
	}
	if (postProcess != DFS_NOP_PROCESS) {
		postProcess(un);
	}
	color[u] = DFS_BLACK;
}

void DFS::runStandardDFS(Graph *g, void (*preProcess)(Node *), void (*preExplore)(Node *, Node *),
						 void (*postExplore)(Node *, Node *), void (*postProcess)(Node *)) {
	uint *color = new uint[g->getOrder()];
	for (uint u = 0; u < g->getOrder(); u++) {
		color[u] = DFS_WHITE;
	}
	for (uint u=0; u < g->getOrder(); u++) {
		if (color[u] == DFS_WHITE) {
			process_standard(g, preProcess, preExplore, postExplore, postProcess, color, u);
		}
	}
	delete[] color;
}

DFS::Inplace::Inplace(uint *graph, uint v, UserFunc1 pre, UserFunc1 post){
	this->g = graph;
	this->startVertex = v;
	this->preProcess = pre;
	this->postProcess = post;
}

uint DFS::Inplace::findStartVertex(uint p){
	unsigned int order = g[0];
	unsigned int numedges = g[0] + 1;
	for(unsigned int i = order + 2; i < numedges; ++i){
		if(g[i] == p){
			return i;
		}
	}
	return 0;	
}

uint DFS::Inplace::name(uint p){
	if(g[p] != 0 && g[p] <= g[0]){
		return g[p];
	}else{
		return 0; 
	}
}

uint& DFS::Inplace::access(uint p){
	if(g[p] == 0 || g[p] > g[0]){
		return g[p];
	}else{
		return g[g[p]];
	}
}

uint& DFS::Inplace::accessStar(uint p){
	if(name(p - 1) == 0 || (0 < access(p) && access(p) <= g[0])){
		return access(p);
	}else if(name(p - 1) != 0){
		return access(access(p)-1);
	}else{
		return access(access(p));
	}
}

void DFS::Inplace::visit(uint p){
	//preprocess
	nextNeighbor(p,true);
}

void DFS::Inplace::nextNeighbor(uint p,bool firstcheck){
	if(firstcheck){
		if(name(p) == 0){
			if(isWhite(p)){
				gotoChild(p);
			}else{
				nextNeighbor(p+1,true);
			}
		}else{
			gotoParent(q);
		}
	}else{
		if(isWhite(p)){
			gotoChild(p);
		}else{
			nextNeighbor(p+1,true);
		}
	}
}

bool DFS::Inplace::isWhite(uint p){
	return g[access(p)] <= g[0];
}

void DFS::Inplace::gotoChild(uint p){
	uint q = accessStar(p);
	uint v = name(q);
	uint x = access(q + 1);
	access(p) = x;
	if(!name(p)){
		access(q+1) = p + 1;
	}else{
		access(q+1) = p;
	}
	visit(q);
}

void DFS::Inplace::gotoParent(uint q){
	uint p = access(q+1);
	access(q+1) = access(p)+1;
	access(p) = q;
	if(name(p-1)!= 0){
		if(accessStar(p-1) < accessStar(p)){
			uint temp = accessStar(p-1);
			accessStar(p-1) = accessStar(p);
			accessStar(p) = temp;
			nextNeighbor(p,true);
		}else{
			nextNeighbor(p+1,true);
	}else{
		nextNeighbor(q+1,true);
	}
}
}

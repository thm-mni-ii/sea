#include "sealib/dfs.h"
#include <stdio.h>

// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
void DFS::process_standard(Graph *g,
	UserFunc1 preProcess, UserFunc2 preExplore,
	UserFunc2 postExplore, UserFunc1 postProcess,
	uint *color, uint u) {
	color[u] = DFS_GRAY;
	Node *un = g->getNode(u);
	if(preProcess != DFS_NOP_PROCESS) preProcess(un);
	for(uint k = 0; k < un->getDegree(); k++) {
		uint v = g->head(u,k);
		Node *vn = g->getNode(v);
		if(preExplore != DFS_NOP_EXPLORE) {
			preExplore(un, vn);
		}
		if(color[v] == DFS_WHITE) {
			process_standard(g, preProcess, preExplore, postExplore, postProcess, color, v);
		}
		if(postExplore != DFS_NOP_EXPLORE) {
			postExplore(un, vn);
		}
	}
	if(postProcess != DFS_NOP_PROCESS) {
		postProcess(un);
	}
	color[u] = DFS_BLACK;
}

void DFS::runStandardDFS(Graph *g, void (*preProcess)(Node *), void (*preExplore)(Node *, Node *),
			void (*postExplore)(Node *, Node *), void (*postProcess)(Node *)) { 
	uint *color = new uint[g->getOrder()];
	for(uint u = 0; u < g->getOrder(); u++) {
		color[u] = DFS_WHITE;
	}
	for(uint u=0; u < g->getOrder(); u++) {
		if(color[u] == DFS_WHITE) {
			process_standard(g, preProcess, preExplore, postExplore, postProcess, color, u);
		}
	}
	delete[] color;
}

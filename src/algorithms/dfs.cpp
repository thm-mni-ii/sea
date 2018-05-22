#include "sealib/algorithms/dfs.h"

void DFS::run() {
	for(unsigned int u=0; u<g->getOrder(); u++) color[u]=DFS_WHITE;
	for(unsigned int u=0; u<g->getOrder(); u++) {
		if(color[u]==DFS_WHITE) process(u);
	}
}

void DFS::process(uint u) { 
	color[u]=DFS_GRAY;
	Node *un=g->getNode(u);
	preProcess(un);
	push(un);
	for(unsigned int k=0; k<un->getDegree(); k++) {
		unsigned int v=g->head(u,k);
		Node *vn=g->getNode(v);
		preExplore(un,vn);
		if(color[v]==DFS_WHITE) process(v);
		postExplore(un,vn);
	}
	postProcess(un);
	color[u]=DFS_BLACK;
}

void DFS::push(Node *u) { stack[sp++]=u; }
Node * DFS::pop() { return stack[--sp]; }
Node * DFS::peek() { return stack[sp-1]; }

DFS::DFS(Graph *p,void (*preprocess)(Node *),void (*preexplore)(Node *,Node *),
			void (*postexplore)(Node *,Node *),void (*postprocess)(Node *)) { 
	g=p;
	color=new unsigned int[g->getOrder()];
	stack=new Node*[g->getOrder()];
	sp=0;
	if(preprocess) preProcess=preprocess;
	if(preexplore) preExplore=preexplore;
	if(postexplore) postExplore=postexplore;
	if(postprocess) postProcess=postprocess;
}

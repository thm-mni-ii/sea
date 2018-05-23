#include "sealib/dfs.h"
#include <stdio.h>
// starting point of the DFS algorithm: O(n+m) time, O(n*log n) bits
// TODO step 1: O((n+m)*log(n)) time, O((ld(3)+e)*n) bits
// TODO step 2: O(n+m) time, O(n*log log n) bits

void DFS::run() {
	for(uint u=0; u<g->getOrder(); u++) {
		if(color[u]==DFS_WHITE) process(u);
	}
}

void DFS::process(uint u) {
	#ifdef DFS_DEBUG
	printf("process %u\n",u);
	#endif
	color[u]=DFS_GRAY;
	Node *un=g->getNode(u);
	#ifdef DFS_DEBUG
	printf("preprocess %u:\n",u);
	#endif
	preProcess(un);
	for(uint k=0; k<un->getDegree(); k++) {
		#ifdef DFS_DEBUG
		printf("head %u,%u = ",u,k);
		#endif
		uint v=g->head(u,k);
		#ifdef DFS_DEBUG
		printf("%u\n",v);
		#endif
		Node *vn=g->getNode(v);
		#ifdef DFS_DEBUG
		printf("preexplore %u,%u:\n",u,v);
		#endif
		preExplore(un,vn);
		if(color[v]==DFS_WHITE) process(v);
		#ifdef DFS_DEBUG
		printf("postexplore %u,%u:\n",u,v);
		#endif
		postExplore(un,vn);
	}
	#ifdef DFS_DEBUG
	printf("postprocess %u:\n",u);
	#endif
	postProcess(un);
	color[u]=DFS_BLACK;
}

void DFS::push(Node *u) { stack[sp++]=u; }
Node * DFS::pop() {
	if(sp==0) return NULL;
	else return stack[--sp];
}
Node * DFS::peek() { return stack[sp-1]; }

uint DFS::getColor(uint u) { 
	if(u>g->getOrder()) return -1;
	else return color[u]; 
}

void DFS::nop() {}

DFS::DFS(Graph *p,void (*preprocess)(Node *),void (*preexplore)(Node *,Node *),
			void (*postexplore)(Node *,Node *),void (*postprocess)(Node *)) { 
	g=p;
	color=new uint[g->getOrder()];
	for(uint u=0; u<g->getOrder(); u++) color[u]=DFS_WHITE;
	stack=new Node*[g->getOrder()];
	sp=0;
	preProcess=preprocess?preprocess:(void(*)(Node*))nop;
	preExplore=preexplore?preexplore:(void(*)(Node*,Node*))nop;
	postExplore=postexplore?postexplore:(void(*)(Node*,Node*))nop;
	postProcess=postprocess?postprocess:(void(*)(Node*))nop;
}

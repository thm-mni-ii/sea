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
void DFS::process_small(uint node,
	Graph *g,CompactArray *color,
	UserFunc1 preProcess,UserFunc2 preExplore,
	UserFunc2 postExplore,UserFunc1 postProcess,
	double epsilon) {
	unsigned n=g->getOrder();
	unsigned q=(unsigned)ceil(n/log(n)*epsilon/6); //2q entries on S shall take up at most (e/3)n bits
	#ifdef DFS_DEBUG
	printf("q=%u, n=%u, (e/3)n=%.0f\n",q,n,(1.5/3)*n);
	#endif
	Stack *st=new Stack((unsigned)ceil(n/(double)q)),
		*kt=new Stack((unsigned)ceil(n/(double)q));
	Stack *s1=new Stack(q),*s2=new Stack(q);
	Stack *k1=new Stack(q),*k2=new Stack(q);
	/*for(uint a=1; a<=n; a++) {
		if(a%(q+1)==q||a==n) trailers->push(a);
		uint b=tryPush(a,s1,s2);
		if(b!=STACK_FAULT) trailers->push(b);
	}
	while(trailers->peek()!=STACK_FAULT) {
		printf(" %u \n",trailers->pop());
	}
	printf("Top segment:\n");
	while(s2->peek()!=STACK_FAULT) {
		printf(" %u \n",s2->pop());
	}
	printf("Bottom segment:\n");
	while(s1->peek()!=STACK_FAULT) {
		printf(" %u \n",s1->pop());
	}*/
	tryPush(node,s1,s2,st);
	tryPush(1,k1,k2,kt);
	uint u,k;
	while(st->peek()!=STACK_FAULT) {
		u=tryPop(s1,s2,st);
		if(u==STACK_FAULT) {
			#ifdef DFS_DEBUG
			printf("no more nodes\n");
			#endif
			return;
		}
		k=tryPop(k1,k2,kt);
		color->insert(u,DFS_GRAY);
		Node *un=g->getNode(u);
		preProcess(un);
		#ifdef DFS_DEBUG
		printf("u: %u(%p), k: %u\n",u,(void*)un,k);
		#endif
		if(k<un->getDegree()) {
			tryPush(u,s1,s2,st);
			tryPush(k+1,k1,k2,kt);
			uint v=g->head(u,k);
			if(color->get(v)==DFS_WHITE) {
				Node *vn=g->getNode(v);
				preExplore(un,vn);
				if(color->get(v)==DFS_WHITE) {
					tryPush(v,s1,s2,st);
					tryPush(1,k1,k2,kt);
				}
				postExplore(un,vn);
			}
		} else {
			color->insert(u,DFS_BLACK);
		}
		postProcess(un);
	}
}
void DFS::tryPush(uint u,Stack *low,Stack *high,Stack *trailers) {
	#ifdef DFS_DEBUG
	printf("-> tryPush %u ",u);
	#endif
	if(!low->isFull()) {
		#ifdef DFS_DEBUG
		printf("(low segment)\n");
		#endif
		low->push(u);
		#ifdef DFS_DEBUG
		printf(" trailer %u => %u\n",trailers->peek(),u);
		#endif
		trailers->pop();
		trailers->push(u);
	}
	else if(!high->isFull()) {
		high->push(u);
		trailers->pop(); trailers->push(u);
	}
	else {
		delete low;
		low=high;
		high=new Stack(low->getSize());
		high->push(u);
	}
}
uint DFS::tryPop(Stack *low, Stack *high,Stack *trailers) { //fix
	#ifdef DFS_DEBUG
	printf("-> tryPop ");
	#endif
	uint r=STACK_FAULT;
	if(high->peek()!=STACK_FAULT) {
		r=high->pop();
	} else if(low->peek()!=STACK_FAULT) {
		#ifdef DFS_DEBUG
		printf("(low segment)\n");
		#endif
		r=low->pop();
	} else {
		if(trailers->peek()!=STACK_FAULT) {
			#ifdef DFS_DEBUG
			printf("(restoring ... ");
			#endif
			// TODO restore
			#ifdef DFS_DEBUG
			printf("done)\n");
			#endif
		} else r=STACK_FAULT;
	}
	#ifdef DFS_DEBUG
	printf("<- tryPop = %u\n",r);
	#endif
	return r;
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
void DFS::runSmallDFS(Graph *g,void (*preProcess)(Node *),void (*preExplore)(Node *,Node *),
			void (*postExplore)(Node *,Node *),void (*postProcess)(Node *)) {
	unsigned n=g->getOrder();
	double e=n%2==0?1.5:3; //assume that 3/e is an integer that divides n
	CompactArray *color=new CompactArray(n,e);
	for(uint a=0; a<g->getOrder(); a++) color->insert(a,DFS_WHITE);
	process_small(0,g,color,preProcess,preExplore,postExplore,postProcess,e);
	delete color;
}

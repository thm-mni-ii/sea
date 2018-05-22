#ifndef DFS_H
#define DFS_H
#include <stdlib.h>
#include "sealib/graph.h"
#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
typedef unsigned int uint;
class DFS {
	private:
		Graph *g;
		unsigned int *color;
		Node **stack;
		int sp;
		void (*preProcess)(Node *);
		void (*preExplore)(Node *,Node *);
		void (*postExplore)(Node *,Node *);
		void (*postProcess)(Node *);

	public:
		DFS(Graph *p,void (*preprocess)(Node *),
			void (*preexplore)(Node *,Node *),
			void (*postexplore)(Node *,Node *),
			void (*postprocess)(Node *));
		void run();
		void process(uint u);
		
		void push(Node *u);
		Node * pop();
		Node * peek();
		static void nop();
		static void nop1(Node*);
		static void nop2(Node*,Node*);
};		
#endif

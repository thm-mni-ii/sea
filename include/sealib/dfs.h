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
		uint *color;
		Node **stack;
		int sp;
		void (*preProcess)(Node *);
		void (*preExplore)(Node *,Node *);
		void (*postExplore)(Node *,Node *);
		void (*postProcess)(Node *);

	public:
		/* Def. DFS
			DFS.p: graph G=(V,E) to iterate over
			DFS.[pre|post][process|explore]: user-defined functions
		*/
		DFS(Graph *p,void (*preprocess)(Node *),
			void (*preexplore)(Node *,Node *),
			void (*postexplore)(Node *,Node *),
			void (*postprocess)(Node *));
			
		/* Def. run
			- Run a depth-first search over the given graph, using the given user functions.
		*/
		void run();
		
		/* Def. process
			process.u: vertex (in V) to be processed
		*/
		void process(uint u);
		
		void push(Node *u);
		Node * pop();
		Node * peek();
		
		uint getColor(uint u);
		
		/* Def. nop 
			- Do nothing. Will be used in place of a NULL user function.
		*/
		static void nop();
};		
#endif

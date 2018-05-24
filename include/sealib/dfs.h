#ifndef DFS_H
#define DFS_H
#include <stdlib.h>
#include "sealib/graph.h"
#include "sealib/stack.h"
#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
typedef void (*UserFunc1)(Node*);
typedef void (*UserFunc2)(Node*,Node*);
typedef unsigned int uint;
class DFS {
	private:

	public:
		DFS();
		/* Def. runStandardDFS
			DFS.p: graph G=(V,E) to iterate over
			DFS.[pre|post][process|explore]: user-defined functions
		*/
		static void runStandardDFS(Graph *g,UserFunc1 preprocess,
			UserFunc2 preexplore,UserFunc2 postexplore,
			UserFunc1 postprocess);
		
		/* Def. process_standard
			process.u: vertex (in V) to be processed
		*/
		static void process_standard(Graph *g,UserFunc1 preProcess,UserFunc2 preExplore,
			UserFunc2 postExplore,UserFunc1 postProcess,uint *color,uint u);
		
		uint getColor(uint u);
		
		/* Def. nop 
			- Do nothing. Will be used in place of a NULL user function.
		*/
		static void nop();
};		
#endif

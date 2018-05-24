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
		/* @param u: vertex (in V) to be processed
		*/
		static void process_standard(Graph *g,UserFunc1 preProcess,UserFunc2 preExplore,
			UserFunc2 postExplore,UserFunc1 postProcess,uint *color,uint u);
			
	public:
		
		/* runStandardDFS:
			Run a standard depth-first search over a graph. <br>
			EFFICIENCY: O(n+m) time, O(n log n) bits
			@param g graph G=(V,E) to iterate over
			@param [pre|post][Process|Explore]: user-defined functions <br>
				to be executed before/after processing a node/exploring an edge
		*/
		static void runStandardDFS(Graph *g,UserFunc1 preprocess,
			UserFunc2 preexplore,UserFunc2 postexplore,
			UserFunc1 postprocess);
		
		/* nop:
			Do nothing. Will be used in place of a NULL user function.
		*/
		static void nop();
};		
#endif

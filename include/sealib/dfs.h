#ifndef SEALIB_DFS_H_
#define SEALIB_DFS_H_

#include "sealib/graph.h"
#include "sealib/node.h"

#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
#define DFS_NOP_PROCESS (void(*)(Node*))0
#define DFS_NOP_EXPLORE (void(*)(Node*, Node*))0

typedef void (*UserFunc1)(Node*);
typedef void (*UserFunc2)(Node*, Node*);
typedef unsigned int uint;

/**	
 * This class contains depth-first search algorithms.
 * The depth-first search of a graph processes all its nodes and
 * explores all its edges.
 * During this procedure, the nodes will be colored
 * from white (initial) to gray (being processed) to black (finished processing)
 * 
 * The following DFS variants are available
 *  - StandardDFS: the normal DFS, uses implicit recursion stack
 * 
 * @author Simon Heuser
 */
class DFS {
 private:
	/**
     * @param u: vertex (in V) to be processed
     */
	static void process_standard(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
								 UserFunc2 postExplore, UserFunc1 postProcess, uint *color, uint u);

	class Inplace{
		private:
			uint *g;
			uint startVertex;
			UserFunc1 preProcess;
			UserFunc1 postProcess;
			uint findStartVertex(uint v);
			void visit(uint p);
			uint& accessStar( uint p);
			void nextNeighbor(uint p, bool firstcheck);
			bool isWhite(uint p);
			void gotoChild(uint p);
			void gotoParent(uint q);
			uint& access(uint p);
			uint name(uint p);
		public:
			Inplace(uint *graph, uint v, UserFunc1 pre, UserFunc1 post);
	};
 public:
	/**
     * Run a standard depth-first search over a graph. <br>
     * EFFICIENCY: O(n+m) time, O(n log n) bits
     * @param g graph G=(V,E) to iterate over
     * @param [pre|post][Process|Explore]: user-defined functions <br>
     * to be executed before/after processing a node/exploring an edge
     * @param preprocess to be executed before processing a node u
     * @param preexplore to be executed before exploring an edge (u,v)
     * @param postexplore to be executed after exploring an edge (u,v)
     * @param postprocess to be executed after processing a node u
     */
	static void runStandardDFS(Graph *g, UserFunc1 preprocess,
							   UserFunc2 preexplore, UserFunc2 postexplore, UserFunc1 postprocess);
	/** 
	 * Run an inplace depth-first search over a directed graph.
	 * EFFICIENCY: O(n+m) time, O(n + m + 2) words
	 * @param g Graph G=(V,E) to iterate over
	 * @param v Vertex to start the DFS from
	 * @param [pre|post][Process]: user-defined functions
	 * to be executed before/after processing a node
   * @param preprocess to be executed before processing a node u
   * @param postprocess to be executed after processing a node u
	 */
	static void runInplaceDirectedDFS(Graph *g,uint v, UserFunc1 preprocess,
								 UserFunc1 postprocess);
};
#endif  // SEALIB_DFS_H_


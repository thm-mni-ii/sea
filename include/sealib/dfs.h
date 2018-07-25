#ifndef SEALIB_DFS_H_
#define SEALIB_DFS_H_

#include "sealib/graph.h"
#include "sealib/node.h"

#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
#define DFS_NOP_PROCESS (void(*)(Node*))0
#define DFS_NOP_EXPLORE (void(*)(Node*, Node*))0

using Sealib::Node;

typedef void (*UserFunc1)(Node*);
typedef void (*UserFunc2)(Node*, Node*);

typedef unsigned int uint;

namespace Sealib {
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


	// TODO: write documentation!
	static void runLinearTimeInplaceDFS(unsigned int* graph, unsigned int startVertex);
};
}  // namespace Sealib
#endif  // SEALIB_DFS_H_


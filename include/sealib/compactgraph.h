#ifndef SEALIB_COMPACTGRAPH_H_
#define SEALIB_COMPACTGRAPH_H_

#include <memory>
#include "sealib/graph.h"

namespace Sealib {
/**
 * A undirected graph G = (V, E) with nodes V = [0, n - 1].
 * The graph stores its Nodes and Edges inside one coherent array
 * Order is stored in A[0]
 * Pointers to adjacency arrays are stored in A[1...order]
 * Size is stored in A[order + 1]
 * Adjacency arrays are stored in A[(order + 2)...(order + size + 2)]
 * @complexity A graph with n nodes and m edges is stored in O(n + m + 2) words
 * @author Simon Schniedenharn 
 */
class Compactgraph : public Graph {
	private:
		std::unique_ptr<unsigned int[]> A;

	public:
		/**
		 * Creates a compactgraph object out of a pointer to an unsigned int array
		 * @param A pointer to a graph array in standard representation
		 */
		explicit Compactgraph(unsigned int _A[]);

    /**
    * Returns the degree of the node that v points at.
    * @param u Vertex u 
    * @return Returns d that is the degree of node v.
    */
		unsigned int getNodeDegree(unsigned int u) override;

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return Returns v that is the k-th neighbor of u.
    */
    unsigned int head(unsigned int u, unsigned int k) override;

    /**
    * @return Returns the order of the graph, i.e, the total number of vertices.
    */
    unsigned int getOrder() override;

		/**
		 * @return Returns the underlying array
		 */
		unsigned int* getData();
};

}  // namespace Sealib
#endif  // SEALIB_COMPACTGRAPH_H_

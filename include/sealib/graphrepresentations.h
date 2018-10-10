#ifndef SEALIB_GRAPHREPRESENTATIONS_H_
#define SEALIB_GRAPHREPRESENTATIONS_H_
#include <random>
#include <iostream>
#include "sealib/compactgraph.h"
/**
 * This class contains static functions to change the
 * representation of graph structures and to generate
 * random graphs.
 *
 * The following transformation variants are available:
 * graph object 	-> standard
 * standard 			-> graph object
 * standard				-> cross pointer
 * standard			 	-> begin pointer
 * cross pointer 	-> swapped cross pointer
 * begin pointer 	-> swapped begin pointer
 *
 * @author Simon Schniedenharn
 */
namespace Sealib {

class Graphrepresentations {
 public:
    static uint* fastGraphGeneration(uint n, uint mPern);

    /**	Generates a Gilbert graph G(n,p) in standard representation with n nodes
     *  and an occurence of every possible edge with a probability of
     *  0 < p < 1
     *  Complexity = O(n^2)
     *
     *	@param numNodes order of the graph
     *	@param p probability of an edge
     *	@param gen engine to generate a pseudo random sequence of numbers
     */
    static Compactgraph *generateGilbertGraph(unsigned int n, double p,
                                              std::mt19937_64 *gen);

    /**	Generates a Gilbert graph G(n,p) in standard representation with n nodes
     *  and an occurence of every possible edge with a probability of
     *  0 < p < 1
     *  Complexity = O(n^2)
     *
     *	@param numNodes order of the graph
     *	@param p probability of an edge
     *	@param gen engine to generate a pseudo random sequence of numbers
     */
    static uint* generateRawGilbertGraph(unsigned int n, double p,
                                         std::mt19937_64 *gen);

    /**
     * Copies a graph object declared in graph.h into the standard
     * representation used for inplace DFS and BFS
     * @param g Graph to be transformed.
     */
    //  static unsigned int* graphToStandard(Graph* g);
    /**
     * Copies a graph from standard representation to a graph object
     * declared in graph.h
     * @param g graph in standard representation
     */
    // static Graph* standardToGraph(unsigned int* g);
    /**
     * Transforms a graph inplace from standard to crosspointer representation
     * @param g graph in standard representation
     * TODO: handle cases of nodes with degree 0 and 1
     */
    static void standardToCrosspointer(unsigned int *g);

    /**
     * Transforms a graph inplace from standard to beginpointer representation
     * @param g graph in standard representation
     */
    static void standardToBeginpointer(unsigned int *g);

    /**
     * Transforms graph inplace from beginpointer to
     * standard representation
     * @param g graph in standard representation
     */
    static void swappedBeginpointerToStandard(unsigned int *g);

    /**
     * Transforms graph inplace from cross or beginpointer to
     * a swapped cross or beginpointer representation
     * @param g graph in cross or beginpointer representation
     */
    static void swapRepresentation(unsigned int *g);

		/**
		 * Transforms graph inplace from standard to
		 * shifted representation.
		 * @param g graph in standard representation
		 */
		static void standardToShifted(unsigned int *g);

		/**
		 * Transforms graph inplace from shifted to
		 * standard representation.
		 * @param g graph in shifted representation
		 */
		static void shiftedToStandard(unsigned int *g);


};
}  // namespace Sealib
#endif // SEALIB_GRAPHREPRESENTATIONS_H_

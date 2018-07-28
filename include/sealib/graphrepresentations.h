#ifndef SEA_LIBS_REPS_H
#define SEA_LIBS_REPS_H
#include "sealib/graph.h"
/**
 * This class contains static functions to change the
 * representation of graph structures.
 *
 * The following transformation variants are available:
 * graph object 	-> standard array
 * standard				-> cross pointer
 * cross pointer 	-> begin pointer
 * cross pointer 	-> swapped cross pointer
 * begin pointer 	-> swapped begin pointer
 *
 * The transformed representations are stored as
 * unsigned integer arrays
 *
 * @author Simon Schniedenharn
 */
using namespace Sealib;

class Graphrepresentations{
	public:
		/**
		 * Generates a graph in standard representation
		 * @param n = number of nodes
		 * @param p = probability of an edge from one node
		 * to another in percent 0 <= p <= 10
		 */
		static unsigned int* generateStandardGraph(unsigned int n,float p);
		/**
		 * Transforms graph object to standard representation
		 * @param g graph G(V,E) to transform
		 */
		static unsigned int* graphToStandard(Graph *g);
		static Graph* standardToGraph(unsigned int* a);
		/**
		 * Transforms graph inplace from standard to 
		 * crosspointer representation
		 * @param g graph in standard representation
		 */
		static void standardToCrosspointer(unsigned int* g);
		/**
		 * Transforms graph inplace from standard to
		 * beginpointer representation
		 * @param g graph in standard representation
		 */
		static void standardToBeginpointer(unsigned int* g);
		/**
		 * Transforms graph inplace from beginpointer to
		 * standard representation
		 * @param g graph in standard representation
		 */
		static void swappedBeginpointerToStandard(unsigned int* g);
		/**
		 * Transforms graph inplace from cross or beginpointer to
		 * a swapped cross or beginpointer representation
		 * @param g graph in cross or beginpointer representation
		 */
		static void swapRepresentation(unsigned int *g);
};
#endif //SEA_LIBS_REPS_H


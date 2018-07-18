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

class Graphrepresentations{
	public:
		/**
		 * Transforms graph object to standard representation
		 * @param g graph G(V,E) to transform
		 */
		static unsigned int* graphToStandard(Graph *g);
		/**
		 * Transforms graph from standard to 
		 * crosspointer representation
		 * @param g graph in standard representation
		 */
		static unsigned int* standardToCrosspointer(unsigned int* g);
		/**
		 * Transforms graph from standard to
		 * beginpointer representation
		 * @param g graph in standard representation
		 */
		static unsigned int* standardToBeginpointer(unsigned int* g);
};
#endif //SEA_LIBS_REPS_H


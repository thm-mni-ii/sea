#ifndef SEALIB_GRAPH_GRAPHREPRESENTATIONS_H_
#define SEALIB_GRAPH_GRAPHREPRESENTATIONS_H_
#include <random>
#include "sealib/graph/compactgraph.h"
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
 * standard				-> shifted
 * shifted 				-> standard
 *
 * @author Simon Schniedenharn
 */
namespace Sealib {

class Graphrepresentations {
 public:
    /**
     * Transforms a graph inplace from standard to crosspointer representation
     * @param g graph in standard representation
     * TODO: handle cases of nodes with degree 0 and 1
     */
    static void standardToCrosspointer(uint32_t *g);

    /**
     * Transforms a graph inplace from standard to beginpointer representation
     * @param g graph in standard representation
     */
    static void standardToBeginpointer(uint32_t *g);

    /**
     * Transforms graph inplace from beginpointer to
     * standard representation
     * @param g graph in standard representation
     */
    static void swappedBeginpointerToStandard(uint32_t *g);

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
#endif  // SEALIB_GRAPH_GRAPHREPRESENTATIONS_H_

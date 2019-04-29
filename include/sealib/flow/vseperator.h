#ifndef SEA_VSEPERATOR_H
#define SEA_VSEPERATOR_H

#include <sealib/collection/bitset.h>
#include <sealib/collection/segmentstack.h>
#include "sealib/graph/directedgraph.h"
#include "sealib/graph/undirectedgraph.h"

namespace Sealib {
/**
 * VSeperator is a class that seperates two Sets in a Graph
 * trough a minimum number of vertices.
 * @Author Vytautas Hermann
 */
class VSeperator{
 public:
    /**
    * Returns a Set of Nodes that are part of the Seperator.
    * @param s first set of vertices
    * @param t second set of vertices
    * @param g the Graph
    * @return Set of vertices, wich disjoin s and t.
    */
    static Sealib::Bitset<> seperate(Sealib::Bitset<> s, Sealib::Bitset<> t, Sealib::Graph const &g, uint64_t k);
};

} // end of Sealib

#endif //VSEA_SEPERATOR_H

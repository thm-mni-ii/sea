#ifndef SEA_VSEPERATOR_H
#define SEA_VSEPERATOR_H

#include <sealib/graph/graph.h>
#include <sealib/collection/bitset.h>

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
        * @return Returns a Set of vertices, wich disjoin s and t.
        */
        Bitset<> seperate(Bitset<> s, Bitset<> t, Graph g);
    };
}

#endif //VSEA_SEPERATOR_H

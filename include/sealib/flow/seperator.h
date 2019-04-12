#ifndef SEA_SEPERATOR_H
#define SEA_SEPERATOR_H

#include <sealib/collection/bitset.h>
#include <sealib/graph/graph.h>

namespace Sealib {
/**
 * Seperator is a virtual class that provides an interface for seperating two Sets in a Graph
 * trough a minimum number of vertices.
 * @Author Vytautas Hermann
 */
    class Seperator {
    public:
        /**
        * Returns a Set of Nodes that are part of the Seperator.
        * @param s first set of vertices
        * @param t second set of vertices
        * @param g the Graph
        * @return Returns a Set of vertices, wich disjoin s and t.
        */
        virtual Bitset<> seperate(Bitset<> s, Bitset<> t, Graph g) const = 0;
    };
}

#endif //SEA_SEPERATOR_H

#ifndef SEA_VSEPERATOR_H
#define SEA_VSEPERATOR_H

#include <sealib/graph/graph.h>
#include <sealib/collection/bitset.h>
#include <sealib/collection/segmentstack.h>
#include "../../../src/flow/vseperator/stgraph.h"

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
    Bitset<> seperate(Bitset<> s, Bitset<> t, Graph g);
 private:
    static Sealib::STGraph graph;
    static bool ispath;
    static uint64_t* path;
    static uint64_t tnode;
    static uint64_t snode;
    static CompactArray *color;
    static SegmentStack *segmentStack;
    static Sealib::Bitset<> s_reachable;
    static void found_t(uint64_t n);
    static void reached(uint64_t n);
};
}

#endif //VSEA_SEPERATOR_H

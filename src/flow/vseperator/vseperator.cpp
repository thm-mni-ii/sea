#include <sealib/flow/vseperator.h>
#include <sealib/graph/directedgraph.h>
#include <sealib/graph/graphcreator.h>
#include <sealib/iterator/dfs.h>
#include <sealib/graph/node.h>
#include "stgraph.h"

using Sealib::VSeperator;

void VSeperator::found_t(uint64_t n){
    if(n == tnode){
        ispath = true;
        while(1) {
            path = (uint64_t *) realloc(path, sizeof(path) + sizeof(uint64_t));
            std::pair<uint64_t, uint64_t> r;
            uint8_t sr = segmentStack->pop(&r);
            if (sr == DFS_DO_RESTORE) {
                Sealib::DFS::restore_top(snode, graph, color, segmentStack);
                segmentStack->pop(&r);

            } else if (sr == DFS_NO_MORE_NODES) {
                return;
            }
            path[sizeof(path) / sizeof(uint64_t) - 1] = r.second;
        }
    }
}

void VSeperator::reached(uint64_t n){
    if(n < graph.getOrder()){
        s_reachable[n] = 1;
    }
}

Sealib::Bitset<> VSeperator::seperate(
        Sealib::Bitset<> s,
        Sealib::Bitset<> t,
        Sealib::Graph g) {
    // create a directed graph g' with in- and output vertices from g
    // sum up the sets s and t to single vertices
    uint64_t s_deg;
    snode = -1;
    tnode = -1;
    std::vector<SimpleNode> nodes;
    nodes.reserve(sizeof(SimpleNode*) * g.getOrder() * 2);

    for(int i = 0; i < g.getOrder(); i++){
        if(s[i] == 1){
            if(snode == -1) snode = i + g.getOrder();
            nodes[snode].addAdjacency(snode - g.getOrder());
            for(int j = 0; j < g.deg(i); j++){
                nodes[snode].addAdjacency(g.head(i, j));
            }
        }
        else if(t[i] == 1){
            if(tnode == -1) tnode = i;
            nodes[tnode].addAdjacency(tnode + g.getOrder());
            for(int j = 0; j < g.deg(i); j++){
                nodes[tnode + g.getOrder()].addAdjacency(g.head(i, j));
            }
        }
        else{
            nodes[i].addAdjacency(i + g.getOrder());
            for(int j = 0; j < g.deg(i); j++){
                nodes[i + g.getOrder()].addAdjacency(g.head(i, j));
            }
        }
    }
    graph = Sealib::STGraph(nodes, snode, s_deg);

    // compute a residental network
    ispath = true;
    while(ispath) {
        ispath = false;
        //---
        CompactArray c(graph.getOrder(), 3);
        color = &c;
        ExtendedSegmentStack stack(graph.getOrder(), graph, color);
        segmentStack = &stack;
        path = (uint64_t*)malloc(0);
        Sealib::DFS::visit_nloglogn(snode, graph, color, segmentStack,
                Sealib::DFS::restore_top, found_t, NULL, NULL, NULL);
        //---
        if(ispath) graph.revertpath(path);
    }

    // compute a set of edges in the minimum cut
    Sealib::DFS::visit_nloglogn(snode, graph, color, segmentStack,
                                Sealib::DFS::restore_top, reached, NULL, NULL, NULL);
    Sealib::Bitset<> vs;
    for(int i = g.getOrder(); i < 2 * g.getOrder(); i++){
        for(int j = 0; j < graph.deg(i); j++){
            if(s_reachable[graph.head(i, j)] == 1 && i - graph.head(i, j) == g.getOrder()) {
                vs[graph.head(i, j)] = 1;
            }
        }
    }
    return vs;
}
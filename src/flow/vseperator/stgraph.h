#ifndef SEA_STGRAPH_H
#define SEA_STGRAPH_H

#include <sealib/graph/graph.h>
#include <sealib/graph/node.h>
#include <vector>

namespace Sealib{
/**
 * The class STGraph should provide a customized graph for the
 * algorithem vseperator to run on.
 * @author Vytautas Hermann
 */
class STGraph: public Graph{
 public:
    /**
     * @param g is the graph stgraph works on
     */
    explicit STGraph(std::vector<SimpleNode> graph, uint64_t s, uint64_t degree):
        g(std::move(graph)), s_pos(s), s_deg(degree) {}

    /**
    * Returns the degree of the node that u points at.
    * @param u Vertex u
    * @return Returns d that is the degree of node v.
    */
    uint64_t deg(uint64_t u) const override {
        if(u == s_pos) return s_deg;
        return g[u].getDegree();
    }

    /**
    * Returns the vertex v that u points at with its k-th edge.
    * @param u Vertex u
    * @param k index in the adjacency vector of node u
    * @return the k-th neighbor of u
    */
    uint64_t head(uint64_t u, uint64_t k) const override { return g[u].getAdj()[k]; }

    /**
    * @return order of the graph, i.e, the total number of vertices.
    */
    uint64_t getOrder() const override { return g.size(); }

    /**
     * Reverts a given path from s to t.
     * s -> v1 -> ... -> vn -> t   =>   s <- v1 <- ... <- vn x t
     * Notice, that s will lose one edge, v1 ... vn will have the same degree and t would grow but because an edge from
     * t to another node won't be used, we're not going to add an edge to t;
     * @param path is the list of position of the next edge of a node
     */
    void revertpath(uint64_t* path){
        uint64_t pos = g[s_pos].getAdj()[path[0]];
        g[s_pos].getAdj()[path[0]] = g[s_pos].getAdj()[s_deg - 1];
        uint64_t mem = g[pos].getAdj()[path[1]];
        uint64_t memnext;
        g[pos].getAdj()[path[1]] = s_pos;
        s_deg--;
        for(int i = 1; i < sizeof(path) / sizeof(uint64_t); i++){
            memnext = g[mem].getAdj()[path[i]+1];
            g[g[pos].getAdj()[path[i]]].getAdj()[path[i]+1] = pos;
            pos = mem;
            mem = memnext;
        }
    }

 private:
    std::vector<SimpleNode> g;
    uint64_t s_deg;
    uint64_t s_pos;
};
}
#endif //SEA_STGRAPH_H

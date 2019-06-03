#ifndef SEA_STGRAPH_H
#define SEA_STGRAPH_H

#include <sealib/graph/graph.h>
#include <sealib/graph/node.h>
#include <vector>

namespace Sealib {
/**
 * The class STGraph should provide a customized graph for the
 * seperator algorithms to run on.
 * @author Vytautas Hermann
 */
class STGraph : public Graph {
 public:
    /**
     * @param graph is the graph stgraph works on
     */
    explicit STGraph(Graph const &graph, Bitset<> s, Bitset<> t) {
        uint64_t s_degree = 0;
        std::vector<std::tuple<uint64_t, uint64_t>> s_edges;
        std::vector<SimpleNode> nodes(graph.getOrder() + 1);

        for (uint64_t i = 0; i < graph.getOrder(); i++) {
            if (t.get(i)) {
                // Edges in t aren't important, because we won't use edges wich
                // are leading away from t. After we reached a vertice in t we
                // stop.
            } else {
                if (s.get(i)) {
                    s_edges.push_back({graph.deg(i) + 1, i});
                    s_degree += graph.deg(i) + 1;
                }
                std::vector<uint64_t> out(graph.deg(i));
                for (uint64_t j = 0; j < graph.deg(i); j++)
                    out[j] = graph.head(i, j);
                nodes[i] = SimpleNode(out);
            }
        }

        std::vector<uint64_t> s_out(s_degree);
        uint64_t k = 0;
        for (uint64_t i = 0; i < s_edges.size(); i++) {
            for (uint64_t j = 0; j < std::get<0>(s_edges[i]); j++) {
                s_out[k] = std::get<1>(s_edges[i]);
            }
        }
        nodes[graph.getOrder()] = SimpleNode(s_out);
        g = nodes;
        s_pos = graph.getOrder();
        s_deg = s_degree;
    }

    /**
     * Returns the degree of the node that u points at.
     * @param u Vertex u
     * @return Returns d that is the degree of node v.
     */
    uint64_t deg(uint64_t u) const override {
        if (u == s_pos) return s_deg;
        return g[u].getDegree();
    }

    /**
     * Returns the vertex v that u points at with its k-th edge.
     * @param u Vertex u
     * @param k index in the adjacency vector of node u
     * @return the k-th neighbor of u
     */
    uint64_t head(uint64_t u, uint64_t k) const override {
        return g[u].getAdj()[k];
    }

    /**
     * @return order of the graph, i.e, the total number of vertices.
     */
    uint64_t getOrder() const override { return g.size(); }

    /**
     * Reverts a given path from s to t.
     * s -> v1 -> ... -> vn -> t   =>   s <- v1 <- ... <- vn x t
     * Notice, that s will lose one edge, v1 ... vn will have the same degree
     * and t would grow but because an edge from t to another node won't be
     * used, we're not going to add an edge to t;
     * @param path is the list of position of the next edge of a node
     */
    void revertpath(std::vector<uint64_t> path) {
        uint64_t pos = s_pos;
        uint64_t mem = g[pos].getAdj()[path[1]];
        g[pos].getAdj()[path[1]] = g[pos].getAdj()[s_deg - 1];
        uint64_t memnext;
        s_deg--;
        for (uint64_t i = 2; i < path.size() - 1; i++) {
            memnext = g[mem].getAdj()[path[i]];
            g[mem].getAdj()[path[i]] = pos;
            pos = mem;
            mem = memnext;
        }
    }

 private:
    std::vector<SimpleNode> g;
    uint64_t s_pos;
    uint64_t s_deg;
};
}  // namespace Sealib
#endif  // SEA_STGRAPH_H

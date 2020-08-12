#ifndef SEALIB_COLLECTION_SIMPLESUBGRAPHSTACK_HPP_
#define SEALIB_COLLECTION_SIMPLESUBGRAPHSTACK_HPP_
#include <sealib/graph/undirectedgraph.h>

#include <unordered_map>
#include <memory>
#include <unordered_set>

#include "bitset.h"

namespace Sealib {

struct HashPair {
    size_t operator()(const std::pair<uint64_t, uint64_t> &p) const {
        auto hash1 = std::hash<uint64_t>{}(p.first);
        auto hash2 = std::hash<uint64_t>{}(p.second);
        return hash1 ^ (hash2 << 32);
    }
};

class SimpleSubGraphStack {
    typedef Sealib::Bitset<uint64_t> bitset_t;
    typedef std::unordered_map<uint64_t, uint64_t> map_t;

 private:
    std::shared_ptr<UndirectedGraph> og_graph;
    std::vector<UndirectedGraph> client_vector;
    std::vector<map_t> from_original_vector;
    std::vector<map_t> to_original_vector;

 public:
    SimpleSubGraphStack(const std::shared_ptr<UndirectedGraph> &graph)
        : og_graph(graph) {}

    void push(const bitset_t &v) {
        uint64_t order = 0;
        map_t from_original;
        map_t to_original;
        uint64_t cnt = 0;
        for (uint64_t i = 0; i < v.size(); i++) {
            if (v.get(i)) {
                if (!client_vector.empty()) {
                    uint64_t k =
                        to_original_vector[client_vector.size() - 1].at(i);
                    from_original.insert(std::make_pair(k, cnt));
                    to_original.insert(std::make_pair(cnt, k));
                } else {
                    from_original.insert(std::make_pair(i, cnt));
                    to_original.insert(std::make_pair(cnt, i));
                }
                cnt++;
                order++;
            }
        }
        auto graph = UndirectedGraph{order};

        std::unordered_set<std::pair<uint64_t, uint64_t>, HashPair> added;
        for (const auto &[key, value] : to_original) {
            const auto &parent_node = og_graph->getNode(value);
            for (const auto &nb : parent_node.getAdj()) {
                if (added.find(std::make_pair(nb.first, value)) ==
                        added.end() &&
                    added.find(std::make_pair(value, nb.first)) ==
                        added.end() && from_original.find(nb.first) != from_original.end()) {
                    auto &first_node = graph.getNode(key);
                    auto &second_node =
                        graph.getNode(from_original.at(nb.first));

                    std::pair<uint64_t, uint64_t> p1 =
                        std::make_pair(key, first_node.getAdj().size());
                    std::pair<uint64_t, uint64_t> p2 =
                        std::make_pair(from_original.at(nb.first),
                                       second_node.getAdj().size());
                    second_node.getAdj().push_back(p1);
                    first_node.getAdj().push_back(p2);
                }
            }
        }
        client_vector.push_back(graph);
        from_original_vector.push_back(from_original);
        to_original_vector.push_back(to_original);
    }

    void push_vertex_induced(const bitset_t &v) {
        push(v);
    }

    void pop() {
        client_vector.pop_back();
        from_original_vector.pop_back();
        to_original_vector.pop_back();
    };

    uint64_t order(uint64_t i) const { 
        if(i == 0) {
            return og_graph->getOrder();
        } else {
            return client_vector[i-1].getOrder();
        }
    }

    inline uint64_t order() const { return order(client_vector.size()); }

    uint64_t degree(uint64_t i, uint64_t u) const {
        if(i == 0) {
            return og_graph->deg(u);
        } else {
            return client_vector[i-1].deg(u);
        }
    }

    inline uint64_t degree(uint64_t u) const {
        return degree(client_vector.size(), u);
    }

    uint64_t head(uint64_t i, uint64_t u, uint64_t k) const {
        if(i == 0) {
            return og_graph->head(u, k);
        } else {
            return client_vector[i-1].head(u, k);
        }
    };

    inline uint64_t head(uint64_t u, uint64_t k) const {
        return head(client_vector.size(), u, k);
    }

    uint64_t mate(uint64_t i, uint64_t u, uint64_t k) const {
        if(i == 0) {
            return og_graph->mate(u, k);
        } else {
            return client_vector[i-1].mate(u, k);
        }
    }

    uint64_t mate(uint64_t u, uint64_t k) const {
        return mate(client_vector.size(), u, k);
    }
};
}
#endif  // SEALIB_COLLECTION_SIMPLESUBGRAPHSTACK_HPP_
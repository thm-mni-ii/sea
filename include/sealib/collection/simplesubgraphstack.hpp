#ifndef SEALIB_COLLECTION_SIMPLESUBGRAPHSTACK_HPP_
#define SEALIB_COLLECTION_SIMPLESUBGRAPHSTACK_HPP_
#include <sealib/graph/undirectedgraph.h>

#include <unordered_map>
#include <memory>
#include <unordered_set>

#include "bitset.h"

namespace Sealib {

typedef struct HashPair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2> &p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 32);
    }
} hash_pair_t;

class SubGraphStack {
    typedef Sealib::Bitset<uint8_t> bitset_t;
    typedef std::unordered_map<uint64_t, uint64_t> map_t;

 private:
    std::shared_ptr<UndirectedGraph> og_graph;
    std::vector<UndirectedGraph> client_vector;
    std::vector<map_t> from_original_vector;
    std::vector<map_t> to_original_vector;

 public:
    explicit SubGraphStack(std::shared_ptr<UndirectedGraph> g_);

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
                    from_original.insert(k, cnt);
                    to_original.insert(cnt, k);
                } else {
                    from_original.insert(i, cnt);
                    to_original.insert(cnt, i);
                }
                cnt++;
                order++;
            }
        }
        auto graph = UndirectedGraph{order};

        std::unordered_set<std::pair<uint64_t, uint64_t>, hash_pair_t> added;
        for (const auto &[key, value] : to_original) {
            const auto &parent_node = og_graph->getNode(value);
            for (const auto &nb : parent_node.getAdj()) {
                if (added.find(std::make_pair(nb.first, value)) ==
                        added.end() &&
                    added.find(std::make_pair(value, nb.first)) ==
                        added.end()) {
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

    void pop() {
        client_vector.pop_back();
        from_original_vector.pop_back();
        to_original_vector.pop_back();
    };

    uint64_t order(uint64_t i) const { return client_vector[i].getOrder(); }

    inline uint64_t order() const { return order(client_vector.size() - 1); }

    uint64_t degree(uint64_t i, uint64_t u) const {
        return client_vector[i].deg(u);
    }

    inline uint64_t degree(uint64_t u) const {
        return degree(client_vector.size() - 1, u);
    }

    uint64_t head(uint64_t i, uint64_t u, uint64_t k) const {
        return client_vector[i].head(u, k);
    };

    inline uint64_t head(uint64_t u, uint64_t k) const {
        return head(client_vector.size() - 1, u, k);
    }

    uint64_t mate(uint64_t i, uint64_t u, uint64_t k) const {
        return client_vector[i].mate(u, k);
    }

    uint64_t mate(uint64_t u, uint64_t k) const {
        return mate(client_vector.size() - 1, u, k);
    }
};
}
#endif  // SEALIB_COLLECTION_SIMPLESUBGRAPHSTACK_HPP_
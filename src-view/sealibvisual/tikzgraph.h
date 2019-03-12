#ifndef SEALIBVISUAL_TIKZGRAPH_H_
#define SEALIBVISUAL_TIKZGRAPH_H_

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include "./tikzedge.h"
#include "./tikzelement.h"
#include "./tikznode.h"

namespace SealibVisual {
/**
 * Representation of a graph. You can add TikzEdges to the graph and accumulate
 * the TEX output this way.
 */
class TikzGraph : public TikzElement {
    using key_t = std::tuple<std::string, std::string>;
    using map_t = std::map<key_t, TikzEdge>;

 public:
    explicit TikzGraph(uint64_t nodes);
    TikzGraph() = default;
    TikzGraph(const TikzGraph &) = default;
    TikzGraph(TikzGraph &&) noexcept = default;
    TikzGraph& operator=(const TikzGraph &) = default;
    // TikzGraph& operator=(TikzGraph &&) noexcept = default;
    virtual ~TikzGraph() = default;

    std::ostream &out(std::ostream &os) const override;

    TikzEdge &addEdge(const key_t &edge, std::string options = "--");

    map_t &getEdges() { return edges; }
    std::vector<TikzNode> &getNodes() { return nodes; }
    bool containsEdge(const key_t &key) const { return edges.count(key) > 0; }

 private:
    map_t edges;
    std::vector<TikzNode> nodes;
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZGRAPH_H_

#ifndef SEALIB_TIKZGRAPH_H_
#define SEALIB_TIKZGRAPH_H_

#include <include/sealibvisual/tikzelement.h>
#include <include/sealibvisual/tikzedge.h>
#include <include/sealibvisual/tikznode.h>
#include <string>
#include <vector>
#include <map>
#include <tuple>

namespace SealibVisual {
/**
 * Used for defining a custom style that can then be used via the given name.
 * Add to a document before using.
 */
class TikzGraph : public TikzElement {
    using key_t = std::tuple<std::string, std::string>;
    using edge_t = SealibVisual::TikzEdge;
    using node_t = SealibVisual::TikzNode;
    using map_t = std::map<key_t, edge_t>;
 private:
    map_t edges;
    std::vector<node_t> nodes;

 public:
    explicit TikzGraph(unsigned int nodes);

    std::ostream &out(std::ostream &os) const override;

    edge_t &addEdge(const key_t &edge);
    map_t &getEdges();
    std::vector<node_t> &getNodes();
    bool containsEdge(const key_t &key) const;
};
}  // namespace SealibVisual
#endif  // SEALIB_TIKZGRAPH_H_

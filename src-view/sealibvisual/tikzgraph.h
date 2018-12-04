#ifndef SEALIBVISUAL_TIKZGRAPH_H_
#define SEALIBVISUAL_TIKZGRAPH_H_

#include "./tikzelement.h"
#include "./tikzedge.h"
#include "./tikznode.h"
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
    using map_t = std::map<key_t, TikzEdge>;
 private:
    map_t edges;
    std::vector<TikzNode> nodes;

 public:
    explicit TikzGraph(unsigned int nodes);

    std::ostream &out(std::ostream &os) const override;

    TikzEdge &addEdge(const key_t &edge, std::string options = "--");
    
    map_t &getEdges() { return edges; }
    std::vector<TikzNode> &getNodes() { return nodes; }
    bool containsEdge(const key_t &key) const { return edges.count(key)>0; }
};
}  // namespace SealibVisual
#endif  // SEALIBVISUAL_TIKZGRAPH_H_

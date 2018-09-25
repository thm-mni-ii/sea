#ifndef SEALIB_SUBGRAPH_H_
#define SEALIB_SUBGRAPH_H_
#include <sealib/graph.h>
#include "rankselect.h"

namespace Sealib {
class SubGraph : Graph{
 private:
    Sealib::RankSelect arcRankSelect;
    Sealib::RankSelect nodeRankSelect;
 public:
 private:
    unsigned int getNodeDegree(unsigned int v) const override;
    unsigned int head(unsigned int u, unsigned int k) const override;
    unsigned int getOrder() const override;
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPHSTACK_H_

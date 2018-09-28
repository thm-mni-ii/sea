#ifndef SEALIB_RECURSIVESUBGRAPH_H_
#define SEALIB_RECURSIVESUBGRAPH_H_
#include <sealib/subgraph.h>
#include <sealib/rankselect.h>

namespace Sealib {
class RecursivSubGraph : public SubGraph {
    typedef SubGraph rgraph_t;
 private:
    rgraph_t *rGraph;
    rankselect_t vSelect;
    rankselect_t aSelect;

 public:
    RecursivSubGraph(SubGraph *rGraph_,
                     Sealib::Bitset<unsigned char> &v,
                     Sealib::Bitset<unsigned char> &e);

    ~RecursivSubGraph() override;
};
}  // namespace Sealib
#endif  // SEALIB_RECURSIVESUBGRAPH_H_

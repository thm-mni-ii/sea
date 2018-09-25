#ifndef SEALIB_SUBGRAPHSTACK_H_
#define SEALIB_SUBGRAPHSTACK_H_

#include <sealib/basicgraph.h>
#include <sealib/subgraph.h>
#include <sealib/linkedliststack.h>
#include <memory>

namespace Sealib {
class SubgraphStack {
    std::shared_ptr<Sealib::BasicGraph> foundation;
    Sealib::LinkedListStack<Sealib::SubGraph> clientList;
};
}  // namespace Sealib
#endif  // SEALIB_SUBGRAPHSTACK_H_

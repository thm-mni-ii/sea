#include "./simplereversedfs.h"
#include <queue>

namespace Sealib {

SimpleReverseDFS::SimpleReverseDFS(Graph const &p) : g(p) {}

void SimpleReverseDFS::init() {
    DFS::nloglognBitDFS(
        g,
        [this](uint64_t u) { result.push(UserCall(UserCall::preprocess, u)); },
        [this](uint64_t u, uint64_t k) {
            result.push(UserCall(UserCall::preexplore, u, k));
        },
        [this](uint64_t u, uint64_t k) {
            result.push(UserCall(UserCall::postexplore, u, k));
        },
        [this](uint64_t u) {
            result.push(UserCall(UserCall::postprocess, u));
        });
    rmax = result.size();
}

bool SimpleReverseDFS::more() { return result.empty(); }

UserCall SimpleReverseDFS::next() {
    UserCall a = result.front();
    result.pop();
    return a;
}

}  // namespace Sealib

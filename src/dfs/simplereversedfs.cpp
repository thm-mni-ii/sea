#include "./simplereversedfs.h"

namespace Sealib {

SimpleReverseDFS::SimpleReverseDFS(Graph const &p1) : g(p1) {}

void SimpleReverseDFS::init() {
    DFS::nloglognBitDFS(
        g,
        [this](uint64_t u) {
            result.push_back(UserCall(UserCall::preprocess, u));
        },
        [this](uint64_t u, uint64_t k) {
            result.push_back(UserCall(UserCall::preexplore, u, k));
        },
        [this](uint64_t u, uint64_t k) {
            result.push_back(UserCall(UserCall::postexplore, u, k));
        },
        [this](uint64_t u) {
            result.push_back(UserCall(UserCall::postprocess, u));
        });
    resultI = result.rbegin();
}

}  // namespace Sealib

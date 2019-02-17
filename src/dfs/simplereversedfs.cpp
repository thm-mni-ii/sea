#include "./simplereversedfs.h"

namespace Sealib {

SimpleReverseDFS::SimpleReverseDFS(Graph const &p1, UserCall::Type p2)
    : g(p1), filter(p2) {}

void SimpleReverseDFS::init() {
    DFS::nloglognBitDFS(
        g,
        [this](uint64_t u) {
            if (filter == UserCall::preprocess || filter == UserCall::nop) {
                result.push_back(UserCall(UserCall::preprocess, u));
            }
        },
        [this](uint64_t u, uint64_t v) {
            if (filter == UserCall::preexplore || filter == UserCall::nop) {
                result.push_back(UserCall(UserCall::preexplore, u, v));
            }
        },
        [this](uint64_t u, uint64_t v) {
            if (filter == UserCall::postexplore || filter == UserCall::nop) {
                result.push_back(UserCall(UserCall::postexplore, u, v));
            }
        },
        [this](uint64_t u) {
            if (filter == UserCall::postprocess || filter == UserCall::nop) {
                result.push_back(UserCall(UserCall::postprocess, u));
            }
        });
    resultI = result.rbegin();
}

}  // namespace Sealib

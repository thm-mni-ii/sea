#ifndef SRC_BFS_SIMPLEBFS_H_
#define SRC_BFS_SIMPLEBFS_H_
#include <queue>
#include <vector>
#include "sealib/iterator/bfs.h"

namespace Sealib {
class SimpleBFS : public Iterator<std::pair<uint64_t, uint64_t>> {
 public:
    SimpleBFS(Graph const &g, Consumer preprocess, BiConsumer preexplore);

    void init() override;

    bool more() override;

    bool nextComponent();

    std::pair<uint64_t, uint64_t> next() override;

    void forEach(
        std::function<void(std::pair<uint64_t, uint64_t>)> f) override {
        do {
            while (more()) f(next());
        } while (nextComponent());
    }

    uint64_t byteSize() const {
        return color.capacity() * sizeof(uint8_t) +
               qmax * sizeof(std::pair<uint64_t, uint64_t>);
    }

 private:
    Graph const &g;
    uint64_t n;
    std::vector<uint8_t> color;
    std::queue<std::pair<uint64_t, uint64_t>> queue;
    uint64_t qmax = 0;
    Consumer preprocess;
    BiConsumer preexplore;
};
}  // namespace Sealib
#endif  // SRC_BFS_SIMPLEBFS_H_

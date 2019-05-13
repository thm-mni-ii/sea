#ifndef SRC_DFS_SIMPLEREVERSEDFS_H_
#define SRC_DFS_SIMPLEREVERSEDFS_H_
#include <queue>
#include "sealib/iterator/reversedfs.h"

namespace Sealib {
/**
 * Simple reverse DFS iterator. Returns representations of the user calls of a
 * normal DFS in reverse order, with an optional filter to select only one type
 * of user call.
 */
class SimpleReverseDFS : public Iterator<UserCall> {
 public:
    /**
     * Create a new reverse DFS iterator.
     * @param g Input graph pointer
     */
    explicit SimpleReverseDFS(Graph const &g);

    /**
     * Initialize the reverse DFS (runs a normal DFS and stores the desired user
     * calls).
     */
    void init() override;

    /**
     * @return true if there are more user calls that can be retrieved
     */
    bool more() override;

    /**
     * Get the next user call object.
     * @return next user call from the reverse sequence
     */
    UserCall next() override;

    uint64_t byteSize() const { return rmax * sizeof(UserCall); }

 private:
    Graph const &g;
    uint64_t rmax = 0;
    std::queue<UserCall> result;
};
}  // namespace Sealib
#endif  // SRC_DFS_SIMPLEREVERSEDFS_H_

#ifndef SRC_DFS_SIMPLEREVERSEDFS_H_
#define SRC_DFS_SIMPLEREVERSEDFS_H_
#include "sealib/iterator/reversedfs.h"

namespace Sealib {
/**
 * Simple reverse DFS iterator. Returns representations of the user calls of a
 * normal DFS in reverse order, with an optional filter to select only one type
 * of user call.
 */
class SimpleReverseDFS : Iterator<UserCall> {
 public:
    /**
     * Create a new reverse DFS iterator.
     * @param g Input graph pointer
     * @param filter Filter to select the desired user call type
     */
    explicit SimpleReverseDFS(Graph const &g,
                              UserCall::Type filter = UserCall::nop);

    /**
     * Initialize the reverse DFS (runs a normal DFS and stores the desired user
     * calls).
     */
    void init() override;

    /**
     * @return true if there are more user calls that can be retrieved
     */
    bool more() override { return resultI != result.rend(); }

    /**
     * Get the next user call object.
     * If a filter is given, only the selected type can appear as output.
     * @return representation of the reverse sequence's next user call
     */
    UserCall next() override { return *resultI++; }

 private:
    Graph const &g;
    UserCall::Type filter;
    std::vector<UserCall> result;
    std::vector<UserCall>::reverse_iterator resultI;
};
}  // namespace Sealib
#endif  // SRC_DFS_SIMPLEREVERSEDFS_H_

#ifndef SEALIB_ITERATOR_REVERSEDFS_H_
#define SEALIB_ITERATOR_REVERSEDFS_H_
#include <limits>
#include <stack>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/graph/graph.h"
#include "sealib/iterator/dfs.h"
#include "sealib/iterator/iterator.h"

namespace Sealib {


/**
 * Reverse DFS iterator which returns a the user calls of a normal DFS in
 * reverse order.
 * To accomplish this in lower space, the forward run of a DFS is split into
 * log(n) intervals and those are simulated in reverse order.
 * As usual, call init() first, and check with more() before getting the next
 * element.
 * @author Simon Heuser
 *
 * EFFICIENCY: O(n+m) time, O(n log(log(n))) bits
 */
class ReverseDFS : Iterator<UserCall>, DFS {
  public:
    explicit ReverseDFS(Graph const &);

    /**
     * Runs a normal DFS to record data about the intervals.
     * EFFICIENCY: O(n+m) time, O(n log(log(n))) bits
     */
    void init() override;

    /**
     * @return true if there are more UserCalls in the DFS
     */
    bool more() override;

    /**
     * Gets the next user call from the reverse DFS. The UserCall struct is
     * shown above.
     * @return next user call
     */
    UserCall next() override;

  private:
    static const std::pair<uint64_t, uint64_t> NIL;
    struct IntervalData {
        std::pair<uint64_t, uint64_t> top =
            NIL;  // top entry at start and end of the interval
        std::pair<uint64_t, uint64_t> bottom =
            NIL;             // value of deepest entry during Ij at time tj
        uint64_t depth = 0;  // index of deepest entry
        UserCall firstCall;
        uint64_t width = 0;  // major-call counter for the interval
    };

    Graph const &g;
    static const uint64_t WORD_SIZE = sizeof(uint64_t) * 8;
    uint64_t n, iCount, iWidth;
    CompactArray c;
    CompactArray d, f;
    ExtendedSegmentStack s;
    std::vector<IntervalData> intervals;
    uint64_t ip = 0;                        // interval pointer
    std::vector<IntervalData>::iterator i;  // interval iterator

    // issued by postexp or postproc if it needs i->top to be filled
    bool needTopOfStack = false;
    // issued by preproc if it needs i->top to be filled
    std::vector<uint64_t> needBelowTop;

    std::vector<UserCall> sequence;
    std::vector<UserCall>::reverse_iterator seqI;

    /**
     * Inserts the missing parts (preexp/postexp) from insertNext until
     * insertLast.
     */
    UserCall insertMinor();
    UserCall latestOutput;
    bool haveNext = false;
    std::pair<uint64_t, uint64_t> insertLast, insertNext;
    UserCall::Type nextType;

    /**
     * Reconstruct sj from the current bottom to top entry.
     * @return the resulting interval stack sj
     */
    std::stack<std::pair<uint64_t, uint64_t>> reconstructStack();
    inline void nextInterval();

    /**
     * Simulate the current interval pointed to by `i`.
     * @throws IntervalStackEmpty if sj is empty and no suitable first user call
     * exists to create an entry
     */
    std::vector<UserCall> simulate(
        std::stack<std::pair<uint64_t, uint64_t>> *sj);
};

class IntervalStackEmpty : public std::exception {
    const char *what() const noexcept {
        return "Reverse DFS (internal error): interval stack is empty";
    }
};
class StackReconstructionFailed : public std::exception {
    const char *what() const noexcept {
        return "Reverse DFS (internal error): could not reconstruct stack, no "
               "node with i < j && f == j found";
    }
};

}  // namespace Sealib
#endif  // SEALIB_ITERATOR_REVERSEDFS_H_

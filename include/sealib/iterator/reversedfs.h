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
 * Representation of a user call. A sequence of these can be retrieved from
 * the ReverseDFS iterator.
 */
struct UserCall {
    enum Type { nop = 0, preprocess, preexplore, postexplore, postprocess };

    /**
     * Create a UserCall object of the given type.
     * @param t Type of the user call
     * @param p1 first number
     * @param p2 second number (optional, depending on user call type)
     */
    UserCall(uint8_t t, uint64_t p1, uint64_t p2 = 0) : type(t), u(p1), k(p2) {}

    constexpr UserCall() : type(nop), u(0), k(0) {}

    bool operator==(UserCall p) const {
        return type == p.type && u == p.u && k == p.k;
    }
    bool operator!=(UserCall p) const { return !(*this == p); }

    uint8_t type;
    uint64_t u, k;
};

/**
 * Reverse DFS iterator which returns a sequence of all the user calls in
 * reverse order.
 * To accomplish this in lower space, the forward run of a DFS is split into
 * log(n) intervals and those are simulated in reverse order. <br>
 * As usual, call init() first, and check with more() before getting the next
 * element.
 *
 * EFFICIENCY: O(n+m) time, O(n log(log(n))) bits
 */
class ReverseDFS : Iterator<UserCall>, DFS {
 public:
    explicit ReverseDFS(Graph const &);

    /**
     * Run a normal DFS to record data about the intervals.
     * EFFICIENCY: O(n+m) time, O(n log(log(n))) bits
     */
    void init() override;

    /**
     * @return true if there are more UserCalls in the DFS
     */
    bool more() override {
        return !(i == intervals.begin() && seqI == sequence.rend());
    }

    /**
     * Get the next user call from the interval.
     * If necessary, first reconstruct the stack and simulate the DFS until the
     * end of the interval.
     * @return next user call from the reverse sequence
     */
    UserCall next() override;

 private:
    struct IntervalData {
        std::pair<uint64_t, uint64_t>
            top;  // top entry at start and end of the interval
        std::pair<uint64_t, uint64_t>
            bottom;  // value of deepest entry during Ij at time tj
        uint64_t height = INVALID, depth = INVALID;  // index of deepest entry
        UserCall firstCall;
        uint64_t width = 0;  // stack-operation counter for the interval
        bool inUse = false;
    };

    Graph const &g;
    static const uint64_t WORD_SIZE = sizeof(uint64_t) * 8;
    uint64_t n, iCount, iWidth;
    CompactArray c;
    CompactArray d, f;
    ExtendedSegmentStack s;
    uint64_t niveau = 0;  // stack height
    std::vector<IntervalData> intervals;
    uint64_t ip = 0;                        // interval pointer
    std::vector<IntervalData>::iterator i;  // interval iterator

    // issued by a user call if it does not know the top entry and left it empty
    bool needTopOfStack = false;

    std::vector<UserCall> sequence;
    UserCall previous;
    std::vector<UserCall>::reverse_iterator seqI;

    std::stack<std::pair<uint64_t, uint64_t>> reconstructStack();
    inline void nextInterval();
    inline void trackSize(uint64_t u0);

    /**
     * Simulate the current interval pointed to by `i`.
     * @throws IntervalStackEmpty if sj is empty and no suitable first user call
     * exists to create an entry
     */
    std::vector<UserCall> simulate(
        std::stack<std::pair<uint64_t, uint64_t>> *sj);

    void process_recording(uint64_t u0);

    void storeTime(bool df, uint64_t u);
    void updateInterval(uint64_t actions, bool end = false);
    void setCall(UserCall call);
};

class IntervalStackEmpty : public std::exception {
    const char *what() const noexcept {
        return "Reverse DFS (internal error): interval stack is empty";
    }
};
class NodeReconstructionFailed : public std::exception {
    const char *what() const noexcept {
        return "Reverse DFS (internal error): could not reconstruct stack, no "
               "node with i<j && f=j found";
    }
};

}  // namespace Sealib
#endif  // SEALIB_ITERATOR_REVERSEDFS_H_

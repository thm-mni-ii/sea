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
    enum Type { preprocess = 0, preexplore, postexplore, postprocess, nop };

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
    bool more() override;

    /**
     * Get the next user call from the interval.
     * If necessary, first reconstruct the stack and simulate the DFS until the
     * end of the interval.
     * @return next user call from the reverse sequence
     */
    UserCall next() override;

 private:
    struct IntervalData {
     public:
        std::pair<uint64_t, uint64_t> h1,
            h2;  // top entries at start and end of the interval
        std::pair<uint64_t, uint64_t> hd;                     // value of deepest entry
        uint64_t hdc = std::numeric_limits<uint64_t>::max();  // index of deepest entry
        UserCall c1 = UserCall();
        uint64_t size = 0;  // call counter for the interval
        bool inUse = false;
    };

    Graph const &g;
    uint64_t n, r, w;
    CompactArray c;
    CompactArray d, f;
    ExtendedSegmentStack s;  // only used for recording run
    uint64_t ns = 0;
    UserCall firstCall;
    std::vector<IntervalData> i;
    uint64_t j = 0;  // interval pointer
    std::vector<UserCall> major;
    UserCall previous;
    std::vector<UserCall>::reverse_iterator majorI;

    void process_recording(uint64_t u0);

    void storeTime(bool df, uint64_t u);
    void updateInterval(uint64_t actions, bool end = false);
    void setCall(UserCall call);

    std::stack<std::pair<uint64_t, uint64_t>> reconstructPart(
        std::pair<uint64_t, uint64_t> from, std::pair<uint64_t, uint64_t> to);

    std::vector<UserCall> simulate(std::stack<std::pair<uint64_t, uint64_t>> *const sj,
                                   std::pair<uint64_t, uint64_t> until, UserCall first);
};

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
#endif  // SEALIB_ITERATOR_REVERSEDFS_H_

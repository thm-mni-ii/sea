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
 public:
    enum Type { preprocess = 0, preexplore, postexplore, postprocess, nop };

    /**
     * Create a UserCall object of the given type.
     * @param t Type of the user call
     * @param p1 first number
     * @param p2 second number (optional, depending on user call type)
     */
    UserCall(unsigned t, uint p1, uint p2 = 0) : type(t), u(p1), k(p2) {}

    constexpr UserCall() : type(nop), u(0), k(0) {}

    bool operator==(UserCall p) const {
        return type == p.type && u == p.u && k == p.k;
    }
    bool operator!=(UserCall p) const { return !(*this == p); }

    unsigned type;
    uint u, k;
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
    explicit ReverseDFS(Graph const *);
    ~ReverseDFS();

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
        std::pair<uint, uint> h1,
            h2;  // top entries at start and end of the interval
        std::pair<uint, uint> hd;                     // value of deepest entry
        uint hdc = std::numeric_limits<uint>::max();  // index of deepest entry
        UserCall c1 = UserCall();
        uint size = 0;  // call counter for the interval
        bool inUse = false;
    };

    Graph const *g;
    uint n, r, w;
    CompactArray c;
    CompactArray d, f;
    ExtendedSegmentStack s;  // only used for recording run
    uint ns = 0;
    UserCall firstCall;
    IntervalData *i;
    uint j = 0;  // interval pointer
    std::vector<UserCall> major;
    UserCall previous;
    std::vector<UserCall>::reverse_iterator majorI;

    void process_recording(uint u0);

    void storeTime(unsigned df, uint u);
    void updateInterval(uint actions, bool end = false);
    void setCall(UserCall call);

    std::stack<std::pair<uint, uint>> reconstructPart(
        std::pair<uint, uint> from, std::pair<uint, uint> to);

    std::vector<UserCall> simulate(std::stack<std::pair<uint, uint>> *const sj,
                                   std::pair<uint, uint> until, UserCall first);
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
    explicit SimpleReverseDFS(Graph const *g,
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
    Graph const *g;
    UserCall::Type filter;
    std::vector<UserCall> result;
    std::vector<UserCall>::reverse_iterator resultI;
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_REVERSEDFS_H_

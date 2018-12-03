#ifndef SEALIB_SEGMENTSTACK_H_
#define SEALIB_SEGMENTSTACK_H_

#include <utility>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/graph/graph.h"

#define DFS_NO_MORE_NODES (unsigned)11
#define DFS_DO_RESTORE (unsigned)12

namespace Sealib {
/*
  Segment Stack:
  - the segment stack has a low and a high segment
  - the trailer (the last slot) of each segment is kept
  - on push, both segments are filled from low to high
    - if both segments are full, the low segment is dropped and a new segment is
  placed above of the high segment
  - on pop, values are taken from high and then low segment
    - if the segments are empty and there are trailers left, a restoration
  request is issued
    - if the segments are empty and the trailers are empty, there is no more
  data in the stack

  @author Simon Heuser
*/
class SegmentStack {
 public:
    virtual void push(std::pair<uint, uint> u) = 0;
    int pop(std::pair<uint, uint> *r);
    bool isEmpty();
    virtual bool isAligned() = 0;
    virtual ~SegmentStack() = default;

 protected:
    explicit SegmentStack(unsigned segmentSize);

    unsigned q;
    std::vector<std::pair<uint, uint>> low, high;
    unsigned lp, hp, tp;
};

/**
 * A basic segment stack which uses only one trailer. Used for O(n)-bit DFS
 * (full restoration).
 *
 * The segment size can be computed using the ε parameter:
 * 2q entries on the segment stack shall take up at most (ε/3)n bits, thus each
 * segment has a size of ((ε/6)n)/bitsize(ENTRY) bits, where bitsize(x) is the
 * number of bits a type occupies (e.g. bitsize(uint)=32).
 */
class BasicSegmentStack : public SegmentStack {
 public:
    explicit BasicSegmentStack(unsigned segmentSize);

    /**
     * Push a tuple on the stack.
     * @param u Tuple to store on the stack
     *
     */
    void push(std::pair<uint, uint> u) override;
    /**
     * @return true if the restoration is finished (i.e. saved trailer and
     * actual trailer are aligned)
     */
    bool isAligned() override;

    /**
     * Empty the entire stack. (Needed for a full restoration)
     */
    void dropAll();
    /**
     * Save a trailer to survive a full restoration.
     */
    void saveTrailer();

 private:
    std::pair<uint, uint> last;
    std::pair<uint, uint> savedTrailer;
    int alignTarget;
};

/**
 * A more complicated segment stack which uses a stack of trailers. Also
 * includes a segment table and supports big and small vertices.
 */
class ExtendedSegmentStack : public SegmentStack {
 public:
    /**
     * Create a new ExtendedSegmentStack. It needs access to the DFS's graph and
     * the color array. The segment size is n/ld(n) by default.
     * @param size Number of nodes in the graph
     * @param g Directed graph G=(V,E)
     * @param c Compact array holding color values
     */
    ExtendedSegmentStack(uint size, Graph *g, CompactArray *c);

    void push(std::pair<uint, uint> u) override;

    /**
     * @return true if saved trailer and actual trailer are aligned
     */
    bool isAligned() override;

    /**
     * Check if u is labeled with the top segment number (i.e. is table[u]=top?)
     * @param u Node to check
     * @param restoring Refer to the previous top segment? (used when restoring
     * and
     * values are actively pushed)
     */
    bool isInTopSegment(uint u, bool restoring = false);
    /**
     * Get the outgoing edge for a given node. Retrieve the approximation from
     * the table if u is small, get the edge from the trailer stack if u is big.
     * @param u Node to get the edge for
     */
    uint getOutgoingEdge(uint u);
    /**
     * Get the second-last trailer (the one that is needed to do a 1-segment
     * restoration).
     * @param r Pointer to the tuple that will hold the trailer
     * @return 0 if a trailer is found, 1 otherwise
     */
    int getRestoreTrailer(std::pair<uint, uint> *r);
    /**
     * Get the last trailer (the one that a restoration will align to).
     * @param r Pointer to the tuple that will hold the trailer
     * @return 0 if a trailer is found, 1 otherwise
     */
    int getTopTrailer(std::pair<uint, uint> *r);
    /**
     * Recolor all vertices in the low segment to the given color (used after a
     * restoration when the low segment is actually the top segment of S).
     * @param v Color to apply to the low segment
     */
    void recolorLow(unsigned v);

    /**
     * Get the edge approximation for a tuple (u,k)
     * @param u Node number
     * @param k Outgoing edge index
     * @return Approximation: f = floor((k-1)/ceil(deg(u)/ld(n)))
     */
    unsigned approximateEdge(uint u, uint k);
    /**
     * Get the outgoing edge index from a given approximation.
     * @param u Node number
     * @param f Approximation
     * @return Outgoing edge: k = ceil(deg(u)/ld(n))*floor((k-1)/gu)
     */
    uint retrieveEdge(uint u, unsigned f);

 private:
    /**
     * A Trailer struct additionally manages a sequence of big vertices.
     * In detail: trailers[tp-1] manages all big vertices in its segment. The
     * first managed vertex can be accessed via big[trailers[tp-1].bi].
     * bc is freely available to increment during restoration.
     */
    struct Trailer {
        std::pair<uint, uint> x;
        unsigned bi, bc;
        Trailer() : bi(INVALID), bc(0) {}
    };

    std::vector<Trailer> trailers;
    unsigned l;
    CompactArray table, edges;
    std::vector<std::pair<uint, uint>> big;
    unsigned bp;
    Graph *graph;
    unsigned m, n;
    CompactArray *color;

    static constexpr unsigned INVALID = static_cast<unsigned>(-1);
    void storeEdges();
};
}  // namespace Sealib
#endif  // SEALIB_SEGMENTSTACK_H_

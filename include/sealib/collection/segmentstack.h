#ifndef SEALIB_COLLECTION_SEGMENTSTACK_H_
#define SEALIB_COLLECTION_SEGMENTSTACK_H_

#include <stdexcept>
#include <utility>
#include <vector>
#include "sealib/_types.h"
#include "sealib/collection/compactarray.h"
#include "sealib/graph/graph.h"

#define DFS_NO_MORE_NODES 11U
#define DFS_DO_RESTORE 12U

#ifdef SEALIBVISUAL_EXAMPLES_H_
namespace SealibVisual {
class VisualDFS;
}
#endif  // SEALIBVISUAL_EXAMPLES_H

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
    /**
     * Push a tuple.
     */
    virtual void push(std::pair<uint64_t, uint64_t> u) = 0;
    /**
     * Pop a tuple.
     * @param r address where the tuple will be stored
     * @return 0 (on success), DFS_NO_MORE_NODES or DFS_DO_RESTORE
     */
    uint8_t pop(std::pair<uint64_t, uint64_t> *r);
    std::pair<uint64_t, uint64_t> top();
    bool isEmpty();
    /**
     * @return number of tuples on the entire stack
     */
    uint64_t size();
    virtual bool isAligned() = 0;
    virtual ~SegmentStack() = default;

 protected:
    explicit SegmentStack(uint64_t segmentSize);

    uint64_t q;
    std::vector<std::pair<uint64_t, uint64_t>> low, high;
    uint64_t lp, hp, tp;
};

/**
 * A basic segment stack which uses only one trailer. Used for O(n)-bit DFS
 * (full restoration).
 *
 * The segment size can be computed using the ε parameter:
 * 2q entries on the segment stack shall take up at most (ε/3)n bits, thus each
 * segment has a size of ((ε/6)n)/bitsize(ENTRY) bits, where bitsize(x) is the
 * number of bits a type occupies (e.g. bitsize(uint64_t)=32).
 */
class BasicSegmentStack : public SegmentStack {
 public:
    explicit BasicSegmentStack(uint64_t segmentSize);

    /**
     * Push a tuple on the stack.
     * @param u Tuple to store on the stack
     */
    void push(std::pair<uint64_t, uint64_t> u) override;
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
    std::pair<uint64_t, uint64_t> last;
    std::pair<uint64_t, uint64_t> savedTrailer;
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
    ExtendedSegmentStack(uint64_t size, Graph const &g, CompactArray *c);

    void push(std::pair<uint64_t, uint64_t> u) override;

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
    bool isInTopSegment(uint64_t u, bool restoring = false);
    /**
     * Get the outgoing edge for a given node. Retrieve the approximation from
     * the table if u is small, get the edge from the trailer stack if u is big.
     * @param u Node to get the edge for
     * @throws TrailersEmpty if there are no trailers (should never happen)
     */
    uint64_t getOutgoingEdge(uint64_t u);
    /**
     * Get the second-last trailer (the one that is needed to do a 1-segment
     * restoration).
     * @param r Pointer to the tuple that will hold the trailer
     * @return 0 if a trailer is found, 1 otherwise
     */
    int getRestoreTrailer(std::pair<uint64_t, uint64_t> *r);
    /**
     * Get the last trailer (the one that a restoration will align to).
     * @param r Pointer to the tuple that will hold the trailer
     * @return 0 if a trailer is found, 1 otherwise
     */
    int getTopTrailer(std::pair<uint64_t, uint64_t> *r);
    /**
     * Recolor all vertices in the low segment to the given color (used after a
     * restoration when the low segment is actually the top segment of S).
     * @param v Color to apply to the low segment
     */
    void recolorLow(uint64_t v);
    /**
     * Get the outgoing edge of u which leads to a gray, top-segment node.
     * @param u node to check
     * @param k starting edge to check
     * @return (c,k) a result tuple with
     *    c: are there more possible nodes to find?
     *    k: outgoing edge that points to a gray, top-segment node (if c is
     * false, k is an 'enclosing' edge)
     */
    std::pair<bool, uint64_t> findEdge(uint64_t u, uint64_t k);

    /**
     * Get the edge approximation for a tuple (u,k)
     * @param u Node number
     * @param k Outgoing edge index
     * @return Approximation: f = floor((k-1)/ceil(deg(u)/ld(n)))
     */
    uint64_t approximateEdge(uint64_t u, uint64_t k);
    /**
     * Get the outgoing edge index from a given approximation.
     * @param u Node number
     * @param f Approximation
     * @return Outgoing edge: k = ceil(deg(u)/ld(n))*floor((k-1)/gu)
     */
    uint64_t retrieveEdge(uint64_t u, uint64_t f);

 private:
    /**
     * A Trailer struct additionally manages a sequence of big vertices.
     * In detail: trailers[tp-1] manages all big vertices in its segment.
     * The first managed vertex can be accessed via big[trailers[tp-1].bi].
     * bc is freely available to increment during restoration.
     */
    struct Trailer {
        std::pair<uint64_t, uint64_t> x;
        uint64_t bi, bc;
        Trailer() : bi(INVALID), bc(0) {}
    };

    std::vector<Trailer> trailers;
    uint64_t l;
    CompactArray table, edges;
    std::vector<std::pair<uint64_t, uint64_t>> big;
    uint64_t bp;
    Graph const &graph;
    uint64_t m, n;
    CompactArray *color;

    /**
     * Store edge indices for the low segment at the appropriate places.
     * @throws BigStackFull if the big stack is full (should never happen)
     */
    void storeEdges();

#ifdef SEALIBVISUAL_EXAMPLES_H_
    friend class SealibVisual::VisualDFS;
#endif  // SEALIBVISUAL_EXAMPLES_H
};

class TrailersEmpty : std::exception {
    const char *what() const noexcept {
        return "SegmentStack (internal error): the trailer stack is empty";
    }
};
class BigStackFull : std::exception {
    const char *what() const noexcept {
        return "SegmentStack (internal error): the big-vertex stack is full";
    }
};

}  // namespace Sealib
#endif  // SEALIB_COLLECTION_SEGMENTSTACK_H_

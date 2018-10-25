#ifndef SRC_SEGMENTSTACK_H_
#define SRC_SEGMENTSTACK_H_

#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/graph.h"

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
  virtual int push(Pair u) = 0;
  int pop(Pair *r);
  bool isEmpty();
  virtual bool isAligned() = 0;

 protected:
  explicit SegmentStack(uint32_t segmentSize);
  virtual ~SegmentStack();

  uint32_t q;
  Pair *low, *high;
  uint32_t lp, hp, tp;
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
  explicit BasicSegmentStack(uint32_t segmentSize);

  int push(Pair u) override;
  // is the restoration finished? (i.e. saved trailer and actual trailer are
  // aligned)
  bool isAligned() override;

  // empty the entire stack - needed for a full restoration */
  void dropAll();
  // save a trailer to survive a full restoration */
  void saveTrailer();

 private:
  Pair last;
  Pair savedTrailer;
  int alignTarget;
};

/**
 * A more complicated segment stack which uses a stack of trailers. Also
 * includes a segment table and supports big and small vertices.
 */
class ExtendedSegmentStack : public SegmentStack {
 public:
  // the extended stack needs access to the graph and the color array; the
  // segment size is n/ld(n) by default
  ExtendedSegmentStack(uint size, Graph *g, CompactArray *c);
  ~ExtendedSegmentStack();

  int push(Pair u) override;
  bool isAligned() override;

  // check if u is labeled with the top segment number (i.e. is table[u]=top?)
  // @param restoring refer to the previous top segment (used when restoring and
  // values are actively pushed)
  bool isInTopSegment(uint u, bool restoring = false);
  // get the outgoing edge: retrieve the approximation from the table if u is
  // small, get the edge from the trailer stack if u is big
  uint getOutgoingEdge(uint u);
  // get the second-last trailer (the one that is needed to do a 1-segment
  // restoration)
  // @return 0 if a trailer is found, 1 otherwise
  int getRestoreTrailer(Pair *r);
  // get the last trailer (the one that a restoration will align to)
  int getTopTrailer(Pair *r);
  // recolor all vertices in the low segment to the given color (used after a
  // restoration when the low segment is actually the top segment of S)
  void recolorLow(uint32_t v);

  uint32_t approximateEdge(uint u, uint k);
  uint retrieveEdge(uint u, uint32_t f);

 private:
  /**
   * A Trailer struct additionally manages a sequence of big vertices.
   * In detail: trailers[tp-1] manages all big vertices in its segment. The
   * first managed vertex can be accessed via big[trailers[tp-1].bi].
   * bc is freely available to increment during restoration.
   */
  struct Trailer {
    Pair x;
    uint32_t bi, bc;
    Trailer() : bi(INVALID), bc(0) {}
  };

  Trailer *trailers;
  uint32_t l;
  CompactArray *table, *edges;
  Pair *big;
  uint32_t bp;
  Graph *graph;
  uint32_t m, n;
  CompactArray *color;

  static constexpr uint32_t INVALID = static_cast<unsigned>(-1);
  void storeEdges();
};
}  // namespace Sealib
#endif  // SRC_SEGMENTSTACK_H_

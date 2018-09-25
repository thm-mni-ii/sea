#ifndef SRC_SEGMENTSTACK_H_
#define SRC_SEGMENTSTACK_H_

#include <stack>
#include <stdexcept>
#include <tuple>
#include "sealib/_types.h"
#include "sealib/compactarray.h"

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
  virtual bool empty();
  virtual bool isAligned() = 0;

 protected:
  explicit SegmentStack(unsigned segmentSize);
  virtual ~SegmentStack();

  unsigned q;
  Pair *low, *high;
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

  int push(Pair u) override;

  /* empty the entire stack - needed for a full restoration */
  void dropAll();
  /* save a trailer to survive a full restoration */
  void saveTrailer();
  /* is the restoration finished? (i.e. saved trailer and actual trailer are
   * aligned) */
  bool isAligned() override;

 private:
  Pair last;
  Pair savedTrailer;
  int alignTarget;
};

/**
 * A more complicated segment stack which uses a stack of trailers. Also
 * includes a segment table and supports big and small vertices.
 *
 *
 */
class ExtendedSegmentStack : public SegmentStack {
 public:
  ExtendedSegmentStack(uint size, unsigned segmentSize);
  ~ExtendedSegmentStack();

  int push(Pair u) override;
  bool isAligned() override;

 private:
  Pair *trailers;
  unsigned l;
  CompactArray *table;
};
}  // namespace Sealib
#endif  // SRC_SEGMENTSTACK_H_

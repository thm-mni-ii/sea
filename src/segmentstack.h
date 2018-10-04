#ifndef SRC_SEGMENTSTACK_H_
#define SRC_SEGMENTSTACK_H_

#include <stack>
#include <stdexcept>
#include "sealib/_types.h"
#include "sealib/compactarray.h"

#define DFS_NO_MORE_NODES (unsigned)11
#define DFS_DO_RESTORE (unsigned)12

namespace Sealib {
/*
  auxiliary stack, used by the EHKDFS
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
  - instead of using a list of trailers, there is the option to store only the
  last trailer in a variable (used in EHKDFS)

  The segment size can be computed using the ε parameter:
  2q entries on the segment stack shall take up at most (ε/3)n bits, thus each
  segment has a size of ((ε/6)n)/bitsize(ENTRY) bits, where bitsize(x) is the
  number of bits a type occupies (e.g. bitsize(uint)=32).

  @author Simon Heuser
*/
class SegmentStack {
 public:
  /* size: number of stored entries, useTrailers: if true, use stack of
   * trailers; if false, use last pushed entry*/
  explicit SegmentStack(uint size, unsigned segmentSize, bool useTrailers);
  ~SegmentStack();

  int push(Pair u);
  int pop(Pair *r);
  bool empty();
  /* empty the entire stack - needed for a full restoration */
  void dropAll();
  /* save a trailer to survive a full restoration */
  void saveTrailer();
  /* is the restoration finished? (i.e. saved trailer and actual trailer are
   * aligned) */
  bool isAligned();
  std::vector<uint> getTrunk();

 private:
  unsigned q;
  bool t = false;
  Pair *low, *high, *trailers, *trunk;
  Pair last;
  unsigned lp = 0, hp = 0, tp = 0;
  Pair savedTrailer;
  int alignTarget;
};
}  // namespace Sealib
#endif  // SRC_SEGMENTSTACK_H_

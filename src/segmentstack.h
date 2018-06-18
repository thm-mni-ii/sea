#ifndef SRC_SEGMENTSTACK_H_
#define SRC_SEGMENTSTACK_H_

#include <stack>
#include <tuple>

#define DFS_NO_MORE_NODES (unsigned)11
#define DFS_DO_RESTORE (unsigned)12
typedef std::tuple<uint, uint> State;

class SegmentStack {
 private:
  State *low, *high, *trailers;
  bool t=false;
  State last;
  long unsigned lp = 0, hp = 0, tp = 0;
  uint q;
  State savedTrailer;

 public:
  /* size: number of stored entries, useTrailers: if true, use stack of trailers; if false, use last pushed entry*/
  explicit SegmentStack(uint size, uint segmentSize, bool useTrailers);
  ~SegmentStack();

  int push(State u);
  int pop(State *r);
  bool empty();
  void dropAll();
  void saveTrailer();
  bool isAligned();
};

#endif  // SRC_SEGMENTSTACK_H_

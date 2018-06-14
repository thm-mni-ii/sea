#ifndef SRC_SEGMENTSTACK_H_
#define SRC_SEGMENTSTACK_H_

#include <stack>
#include <tuple>
#define DFS_NO_MORE_NODES (unsigned)11
#define DFS_DO_RESTORE (unsigned)12
typedef std::tuple<uint, uint> State;
typedef std::stack<State> Stack;

class SegmentStack {
 private:
  Stack *low, *high, *trailers;
  uint q;
  State savedTrailer;

 public:
  explicit SegmentStack(uint segmentSize);
  ~SegmentStack();

  int push(State u);
  int pop(State *r);
  bool empty();
  void dropAll();
  void saveTrailer();
  bool isAligned();
};

#endif  // SRC_SEGMENTSTACK_H_

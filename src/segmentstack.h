#ifndef SEALIB_SEGMENTSTACK_H_
#define SEALIB_SEGMENTSTACK_H_
#include "sealib/dfs.h"
typedef std::stack<State> Stack;
class SegmentStack {
 private:
  Stack *low, *high, *trailers;
  uint q;

 public:
  SegmentStack(uint segmentSize);
  ~SegmentStack();

  int push(State u);
  int pop(State *r);
  int dropLow();
  bool empty();
  bool isAligned();
};

#endif  // SEALIB_SEGMENTSTACK_H_

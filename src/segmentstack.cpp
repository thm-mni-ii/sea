#include "src/segmentstack.h"

SegmentStack::SegmentStack(uint segmentSize) {
  q = segmentSize;
  low = new Stack;
  high = new Stack;
  trailers = new Stack;
}
SegmentStack::~SegmentStack() {
  delete low;
  delete high;
  delete trailers;
}

int SegmentStack::push(State u) {
// todo: throw DFS_RESTORE_DONE if top(high)==top(trailers)
#ifdef DFS_DEBUG
  printf("-> push (%u,%u) \n", std::get<0>(u), std::get<1>(u));
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", low->size(), high->size(),
         trailers->size());
#endif
  if (low->size() <= q) {
#ifdef DFS_DEBUG
    printf(" (low segment)\n");
#endif
    low->push(u);
    if (low->size() == q) {
#ifdef DFS_DEBUG
      printf(" trailer %p => (%u,%u)\n",
             !trailers->empty() ? &trailers->top() : (void *)0, std::get<0>(u),
             std::get<1>(u));
#endif
      trailers->push(u);
    }
  } else if (high->size() <= q) {
#ifdef DFS_DEBUG
    printf(" (high segment)\n");
#endif
    high->push(u);
    if (high->size() == q) {
#ifdef DFS_DEBUG
      printf(" trailer %p => (%u,%u)\n",
             !trailers->empty() ? &trailers->top() : (void *)0, std::get<0>(u),
             std::get<1>(u));
#endif
      trailers->push(u);
    }
  } else {
#ifdef DFS_DEBUG
    printf(" (drop old segment)\n");
#endif
    delete low;
    low = high;
    high = new Stack;
  }
#ifdef DFS_DEBUG
  printf("<- push \n");
#endif
  return 0;
}
int SegmentStack::pop(State *r) {
#ifdef DFS_DEBUG
  printf("-> pop \n");
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", low->size(), high->size(),
         trailers->size());
#endif
  if (!high->empty()) {
#ifdef DFS_DEBUG
    printf(" (high segment)\n");
#endif
    *r = high->top();
    high->pop();
  } else if (!low->empty()) {
#ifdef DFS_DEBUG
    printf(" (low segment)\n");
#endif
    *r = low->top();
    low->pop();
  } else {
    if (!trailers->empty()) {
      return DFS_DO_RESTORE;
    } else {
      return DFS_NO_MORE_NODES;
    }
  }
#ifdef DFS_DEBUG
  printf("<- pop = (%u,%u)\n", std::get<0>(*r), std::get<1>(*r));
#endif
  return 0;
}
bool SegmentStack::empty() {
  return low->empty() && high->empty() && trailers->empty();
}
bool SegmentStack::isAligned() { return high->top() == trailers->top(); }

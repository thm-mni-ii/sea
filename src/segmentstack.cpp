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
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", low->size(), high->size(),
         trailers->size());
#endif
  if (low->size() < q) {
#ifdef DFS_DEBUG
    printf(" (low segment+1)\n");
#endif
    low->push(u);
    if (low->size() == q) {
#ifdef DFS_DEBUG
      printf(" + trailer (%u,%u)\n", std::get<0>(u), std::get<1>(u));
#endif
      trailers->push(u);
    }
  } else if (high->size() < q) {
#ifdef DFS_DEBUG
    printf(" (high segment+1)\n");
#endif
    high->push(u);
    if (high->size() == q) {
#ifdef DFS_DEBUG
      printf(" + trailer (%u,%u)\n", std::get<0>(u), std::get<1>(u));
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
    high->push(u);
  }
  return 0;
}
int SegmentStack::pop(State *r) {
#ifdef DFS_DEBUG
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", low->size(), high->size(),
         trailers->size());
#endif
  if (!high->empty()) {
#ifdef DFS_DEBUG
    printf(" (high segment-1)\n");
#endif
    if (high->size() == q) {
      trailers->pop();
    }
    *r = high->top();
    high->pop();
  } else if (!low->empty()) {
#ifdef DFS_DEBUG
    printf(" (low segment-1)\n");
#endif
    if (low->size() == q) {
      trailers->pop();
    }
    *r = low->top();
    low->pop();
  } else {
    if (!trailers->empty()) {
      return DFS_DO_RESTORE;
    } else {
      return DFS_NO_MORE_NODES;
    }
  }
  return 0;
}
bool SegmentStack::empty() {
  return low->empty() && high->empty() && trailers->empty();
}
void SegmentStack::dropAll() {
  delete low;
  delete high;
  delete trailers;
  low = new Stack;
  high = new Stack;
  trailers = new Stack;
}
void SegmentStack::saveTrailer() { savedTrailer = trailers->top(); }
bool SegmentStack::isAligned() {
  bool r = false;
  if (high->size() == 0 || trailers->size() == 0)
    r = false;
  else
    r = high->top() == savedTrailer;
  return r;
}

#include "src/segmentstack.h"

SegmentStack::SegmentStack(uint size, uint segmentSize) {
  q = segmentSize;
  low = reinterpret_cast<State *>(malloc(q * sizeof(State)));
  high = reinterpret_cast<State *>(malloc(q * sizeof(State)));
  trailers = reinterpret_cast<State *>(malloc((size / q + 1) * sizeof(State)));
}
SegmentStack::~SegmentStack() {
  delete low;
  delete high;
  delete trailers;
}

int SegmentStack::push(State u) {
  if (lp < q) {
    low[lp++] = u;
    if (lp == q) {
      trailers[tp++] = u;
    }
  } else if (hp < q) {
    high[hp++] = u;
    if (hp == q) {
      trailers[tp++] = u;
    }
  } else {
    State *tmp = low;
    low = high;
    high = tmp;
    hp = 0;
    high[hp++] = u;
  }
  return 0;
}
int SegmentStack::pop(State *r) {
  if (hp > 0) {
    if (hp == q) {
      tp--;
    }
    *r = high[--hp];
  } else if (lp > 0) {
    if (lp == q) {
      tp--;
    }
    *r = low[--lp];
  } else {
    if (tp > 0) {
      return DFS_DO_RESTORE;
    } else {
      return DFS_NO_MORE_NODES;
    }
  }
  return 0;
}
bool SegmentStack::empty() { return lp == 0 && hp == 0 && tp == 0; }
void SegmentStack::dropAll() {
  lp = 0;
  hp = 0;
  tp = 0;
}
void SegmentStack::saveTrailer() { savedTrailer = trailers[tp - 1]; }
bool SegmentStack::isAligned() {
  bool r = false;
  if (hp == 0 || tp == 0)
    r = false;
  else
    r = high[hp - 1] == savedTrailer;
  return r;
}

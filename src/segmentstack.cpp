#include "src/segmentstack.h"
#define DFS_DEBUG

SegmentStack::SegmentStack(uint segmentSize) {
  q = segmentSize;
  low = static_cast<State*>(malloc(q*sizeof(State)));
  high = static_cast<State*>(malloc(q*sizeof(State)));
  trailers = static_cast<State*>(malloc(q*sizeof(State)));
}
SegmentStack::~SegmentStack() {
  delete low;
  delete high;
  delete trailers;
}

int SegmentStack::push(State u) {
// todo: throw DFS_RESTORE_DONE if top(high)==top(trailers)
#ifdef DFS_DEBUG
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", lp, hp, tp);
#endif
  if (lp < q) {
#ifdef DFS_DEBUG
    printf(" (low segment+1)\n");
#endif
    low[lp++]=u;
    if (lp == q) {
#ifdef DFS_DEBUG
      printf(" + trailer (%u,%u)\n", std::get<0>(u), std::get<1>(u));
#endif
      trailers[tp++]=u;
    }
  } else if (hp < q) {
#ifdef DFS_DEBUG
    printf(" (high segment+1)\n");
#endif
    high[hp++]=u;
    if (hp == q) {
#ifdef DFS_DEBUG
      printf(" + trailer (%u,%u)\n", std::get<0>(u), std::get<1>(u));
#endif
      trailers[tp++]=u;
    }
  } else {
#ifdef DFS_DEBUG
    printf(" (drop old segment)\n");
#endif
    State *tmp=low;
    low=high;
    lp=hp;
    high=tmp;
    hp=0;
    high[hp++]=u;
  }
  return 0;
}
int SegmentStack::pop(State *r) {
#ifdef DFS_DEBUG
  printf("|low|=%lu, |high|=%lu, |trailers|=%lu\n", lp, hp, tp);
#endif
  if (hp>0) {
#ifdef DFS_DEBUG
    printf(" (high segment-1)\n");
#endif
    if (hp == q) {
      tp--;
    }
    *r = high[--hp];
  } else if (lp>0) {
#ifdef DFS_DEBUG
    printf(" (low segment-1)\n");
#endif
    if (lp == q) {
      tp--;
    }
    *r = low[--lp];
  } else {
    if (tp>0) {
      return DFS_DO_RESTORE;
    } else {
      return DFS_NO_MORE_NODES;
    }
  }
  return 0;
}
bool SegmentStack::empty() {
  return lp==0 && hp==0 && tp==0;
}
void SegmentStack::dropAll() {
  lp=0;
  hp=0;
  tp=0;
}
void SegmentStack::saveTrailer() { savedTrailer = trailers[tp-1]; }
bool SegmentStack::isAligned() {
  bool r = false;
  if (hp == 0 || tp == 0)
    r = false;
  else
    r = high[hp-1] == savedTrailer;
  return r;
}

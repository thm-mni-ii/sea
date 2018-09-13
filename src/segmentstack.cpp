#include "src/segmentstack.h"

using Sealib::SegmentStack;
using Sealib::CompactArray;

SegmentStack::SegmentStack(uint size, unsigned segmentSize, bool useTrailers)
    : q(segmentSize), t(useTrailers), low(new Pair[q]), high(new Pair[q]) {
  if (t) {
    trailers = new Pair[size / q + 1];
    trunk =
        new Pair[q];  // store the segment below `low` (for partial recoloring)
  } else {
    // use only the last trailer
  }
}
SegmentStack::~SegmentStack() {
  delete[] low;
  delete[] high;
  if (t) delete[] trailers;
}

int SegmentStack::push(Pair u) {
  if (lp < q) {
    low[lp++] = u;
  } else if (hp < q) {
    high[hp++] = u;
  } else {
    if (t) {
      trailers[tp] = low[lp - 1];
      for (unsigned a = 0; a < q; a++) trunk[a] = low[a];
    } else {
      last = low[lp - 1];
    }
    tp++;
    Pair *tmp = low;
    low = high;
    high = tmp;
    hp = 0;
    high[hp++] = u;
  }
  return 0;
}
int SegmentStack::pop(Pair *r) {
  if (hp > 0) {
    *r = high[--hp];
  } else if (lp > 0) {
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
bool SegmentStack::empty() {
  bool r;
  r = lp == 0 && hp == 0 && tp == 0;
  return r;
}

void SegmentStack::dropAll() {
  lp = 0;
  hp = 0;
  tp = 0;
}
void SegmentStack::saveTrailer() {
  if (tp > 0) {
    savedTrailer = t ? trailers[tp - 1] : last;
    alignTarget = tp == 1 ? 1 : 2;
  } else {
    throw std::logic_error("segmentstack: cannot save from empty trailers");
  }
}

std::vector<uint> SegmentStack::getTrunk() {
  std::vector<uint> r;
  for (unsigned a = 0; a < q; a++) {
    r.push_back(std::get<0>(trunk[a]));
  }
  return r;
}

bool SegmentStack::isAligned() {
  bool r = false;
  if ((alignTarget == 2 && hp < q) || lp < q) {
    r = false;
  } else {
    unsigned lu = low[lp - 1].head(), lk = low[lp - 1].tail();
    unsigned hu = 0, hk = 0;
    if (alignTarget == 2) hu = high[hp - 1].head(), hk = high[hp - 1].tail();
    unsigned tu = savedTrailer.head(), tk = savedTrailer.tail();
    r = (alignTarget == 2 && hu == tu && hk == tk) ||
        (alignTarget == 1 && lu == tu && lk == tk);
  }
  return r;
}

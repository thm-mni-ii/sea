#include "src/segmentstack.h"

using Sealib::SegmentStack;

SegmentStack::SegmentStack(uint size, unsigned segmentSize, bool extendedMode)
    : q(segmentSize),
      t(extendedMode),
      low(new Pair[q]),
      high(new Pair[q]),
      l(static_cast<unsigned>(log(size))) {
  if (t) {
    trailers = new Pair[size / q + 1];
    table = new CompactArray(size, q, l);
  } else {
    // use only the last trailer
  }
}
SegmentStack::~SegmentStack() {
  delete[] low;
  delete[] high;
  if (t) {
    delete[] trailers;
    delete table;
  }
}

int SegmentStack::push(Pair u) {
  // if(t) table[u.head()]=tp;
  if (lp < q) {
    low[lp++] = u;
  } else if (hp < q) {
    high[hp++] = u;
  } else {
    if (t) {
      trailers[tp] = low[lp - 1];
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

bool SegmentStack::isAligned() {
  bool r = false;
  if ((alignTarget == 2 && hp < q) || lp < q) {
    r = false;
  } else {
    unsigned lu = std::get<0>(low[lp - 1]), lk = std::get<1>(low[lp - 1]);
    unsigned hu = 0, hk = 0;
    if (alignTarget == 2)
      hu = std::get<0>(high[hp - 1]), hk = std::get<1>(high[hp - 1]);
    unsigned tu = std::get<0>(savedTrailer), tk = std::get<1>(savedTrailer);
    r = (alignTarget == 2 && hu == tu && hk == tk) ||
        (alignTarget == 1 && lu == tu && lk == tk);
  }
  return r;
}

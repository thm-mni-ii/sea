#include "src/segmentstack.h"

using Sealib::SegmentStack;
using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;

//  -- SUPERCLASS --

SegmentStack::SegmentStack(unsigned segmentSize)
    : q(segmentSize),
      low(new Pair[q]),
      high(new Pair[q]),
      lp(0),
      hp(0),
      tp(0) {}

SegmentStack::~SegmentStack() {
  delete[] low;
  delete[] high;
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

//  -- BASIC --

BasicSegmentStack::BasicSegmentStack(unsigned segmentSize)
    : SegmentStack(segmentSize) {}

int BasicSegmentStack::push(Pair u) {
  // if(t) table[u.head()]=tp;
  if (lp < q) {
    low[lp++] = u;
  } else if (hp < q) {
    high[hp++] = u;
  } else {
    last = low[lp - 1];
    tp++;
    Pair *tmp = low;
    low = high;
    high = tmp;
    hp = 0;
    high[hp++] = u;
  }
  return 0;
}

void BasicSegmentStack::dropAll() {
  lp = 0;
  hp = 0;
  tp = 0;
}

void BasicSegmentStack::saveTrailer() {
  if (tp > 0) {
    savedTrailer = last;
    alignTarget = tp == 1 ? 1 : 2;
  } else {
    throw std::logic_error("segmentstack: cannot save from empty trailers");
  }
}

bool BasicSegmentStack::isAligned() {
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

//  -- EXTENDED --

ExtendedSegmentStack::ExtendedSegmentStack(uint size, unsigned segmentSize)
    : SegmentStack(segmentSize),
      trailers(new Pair[size / q + 1]),
      l(static_cast<unsigned>(log(size))),
      table(new CompactArray(size, q, l)) {}

ExtendedSegmentStack::~ExtendedSegmentStack() {
  delete[] trailers;
  delete table;
}

int ExtendedSegmentStack::push(Pair u) {
  if (lp < q) {
    low[lp++] = u;
  } else if (hp < q) {
    high[hp++] = u;
  } else {
    trailers[tp] = low[lp - 1];
    tp++;
    Pair *tmp = low;
    low = high;
    high = tmp;
    hp = 0;
    high[hp++] = u;
  }
  return 0;
}

bool ExtendedSegmentStack::isAligned() { return true; }

#include "src/segmentstack.h"

using Sealib::SegmentStack;

State *low, *high, *trailers;
bool t = false;
State last;
unsigned lp, hp, tp;
uint q;
State savedTrailer;
int alignTarget;

SegmentStack::SegmentStack(uint size, uint segmentSize, bool useTrailers) {
  q = segmentSize;
  t = useTrailers;
  low = new State[q];
  high = new State[q];
  lp = hp = tp = 0;
  if (t) {
    trailers = new State[size / q + 1];
  } else {
    // use only the last trailer
  }
}
SegmentStack::~SegmentStack() {
  delete[] low;
  delete[] high;
  if (t) delete[] trailers;
}

int SegmentStack::push(State u) {
  if (lp < q) {
    low[lp++] = u;
    /*if (lp == q) {
      if (t) trailers[tp] = u;
      tp++;
    }*/
  } else if (hp < q) {
    high[hp++] = u;
    /*if (hp == q) {
      if (t) trailers[tp] = u;
      tp++;
    }*/
  } else {
    if (t)
      trailers[tp] = low[lp - 1];
    else
      last = low[lp - 1];
    tp++;
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
    /*if (hp == q) {
      tp--;
    }*/
    *r = high[--hp];
  } else if (lp > 0) {
    /*if (lp == q) {
      tp--;
    }*/
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

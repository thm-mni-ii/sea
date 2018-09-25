#include "src/segmentstack.h"
#include <iostream>

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

bool SegmentStack::isEmpty() {
  bool r;
  r = lp == 0 && hp == 0 && tp == 0;
  return r;
}

//  -- BASIC --

BasicSegmentStack::BasicSegmentStack(unsigned segmentSize)
    : SegmentStack(segmentSize) {}

int BasicSegmentStack::push(Pair u) {
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

ExtendedSegmentStack::ExtendedSegmentStack(uint size, unsigned segmentSize,
                                           Graph *g, CompactArray *c)
    : SegmentStack(segmentSize),
      trailers(new Pair[size / q + 1]),
      l(static_cast<unsigned>(log(size))),
      table(new CompactArray(size, q, l)),
      edges(new CompactArray(size, q, l)),
      graph(g),
      color(c) {}

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
  uint uv = u.head(), uk = u.tail();
  table->insert(uv, tp);  // ?
  if (graph->getNodeDegree(uv) > graph->getOrder() / q) {
    // to do: add big vertex storage in trailers
  } else {
    double g = ceil(graph->getNodeDegree(uv) / l);
    unsigned f = static_cast<unsigned>(floor((uk - 1) / g));
    edges->insert(uv, f);
  }
  std::cout << "lp=" << lp << ", hp=" << hp << ", tp=" << tp << "\n";
  return 0;
}

bool ExtendedSegmentStack::isInTopSegment(uint u) {
  bool r;
  r = table->get(u) == tp;
  std::cout << "segment(" << u << ") = " << table->get(u) << "\n";
  return r;
}

uint ExtendedSegmentStack::getOutgoingEdge(uint u) {
  if (graph->getNodeDegree(u) > graph->getOrder() / q) {
    // to do: add big vertex storage in trailers
    return 0;
  } else {
    unsigned f = edges->get(u);
    unsigned g = graph->getNodeDegree(u) / l;
    unsigned k1 = f * g;
    return k1;
  }
}

int ExtendedSegmentStack::getRestoreTrailer(Pair *r) {
  if (tp > 1) {
    *r = trailers[tp - 2];
    return 0;
  } else {
    return 1;
  }
}

void ExtendedSegmentStack::recolorLow(uint v) {
  for (unsigned a = 0; a < q; a++) color->insert(low[a].head(), v);
}

bool ExtendedSegmentStack::isAligned() { return lp == q; }

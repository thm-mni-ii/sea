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

ExtendedSegmentStack::ExtendedSegmentStack(uint size, Graph *g, CompactArray *c)
    : SegmentStack(static_cast<unsigned>(ceil(size / log2(size)))),
      trailers(new Trailer[size / q + 1]),
      l(static_cast<unsigned>(ceil(log(size)))+1),
      table(new CompactArray(size, l)),
      edges(new CompactArray(size, l)),
      big(new Pair[q]),
      bp(0),
      graph(g),
      n(graph->getOrder()),
      color(c) {
  m = 0;
  for (uint a = 0; a < n; a++) m += g->getNodeDegree(a);
  std::cout << "q=" << q << "\n";
}

ExtendedSegmentStack::~ExtendedSegmentStack() {
  delete[] trailers;
  delete table;
}

int ExtendedSegmentStack::push(Pair u) {
  uint uv = u.head(), uk = u.tail();
  if (graph->getNodeDegree(uv) > m / q) {
    std::cout << "deg(" << uv << ")=" << graph->getNodeDegree(uv) << "; > "
              << m / q << "\n";
    trailers[tp].bi += 1;  // another big vertex is stored
    big[bp++] = u;
    if (bp > q) throw std::out_of_range("big storage is full!");
  } else if(uk>0) {
    double g = ceil(graph->getNodeDegree(uv) / l);
    unsigned f = static_cast<unsigned>(floor((uk - 1) / g));
    std::cout << "inserting edge(" << uk << ")=" << f << "\n";
    edges->insert(uv, f);
  }
  if(tp>0) table->insert(uv,tp+1);
  if (lp < q) {
    if(tp==0) table->insert(uv, 0);  // ?
    low[lp++] = u;
  } else if (hp < q) {
    if(tp==0) table->insert(uv, 1);  // ?
    high[hp++] = u;
  } else {
    trailers[tp].x = low[lp - 1];
    tp++;
    Pair *tmp = low;
    low = high;
    high = tmp;
    hp = 0;
    high[hp++] = u;
    table->insert(uv,tp+1);
  }
  std::cout << "lp=" << lp << ", hp=" << hp << ", tp=" << tp << ", bp=" << bp
            << ", table(u)=" << table->get(uv) << "\n";
  return 0;
}

bool ExtendedSegmentStack::isInTopSegment(uint u) {
  bool r;
  r = table->get(u) == tp+1;
  std::cout << "segment(" << u << ") = " << table->get(u) << "; tp=" << tp+1 << "\n";
  return r;
}

uint ExtendedSegmentStack::getOutgoingEdge(uint u) {
  if (graph->getNodeDegree(u) > m / q) {
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
    *r = trailers[tp - 2].x;
    return 0;
  } else {
    return 1;
  }
}

void ExtendedSegmentStack::recolorLow(unsigned v) {
  for (unsigned a = 0; a < q; a++) color->insert(low[a].head(), v);
}

bool ExtendedSegmentStack::isAligned() { return lp == q; }

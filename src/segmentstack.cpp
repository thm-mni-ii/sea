#include "sealib/segmentstack.h"
#include <cmath>
#include <stack>
#include <stdexcept>

using Sealib::SegmentStack;
using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;

class E_noTrailers : std::exception {
    const char *what() const throw() {
        return "SegmentStack: the trailer stack is empty, but you called a "
               "method "
               "which needs existing trailers";
    }
};

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
        throw E_noTrailers();
    }
}

bool BasicSegmentStack::isAligned() {
    bool r = false;
    if ((alignTarget == 2 && hp < q) || lp < q) {
        r = false;
    } else {
        unsigned lu = low[lp - 1].head(), lk = low[lp - 1].tail();
        unsigned hu = 0, hk = 0;
        if (alignTarget == 2)
            hu = high[hp - 1].head(), hk = high[hp - 1].tail();
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
      l(static_cast<unsigned>(ceil(log2(size))) + 1),
      table(new CompactArray(size, l)),
      edges(new CompactArray(size, l)),
      big(new Pair[q]),
      bp(0),
      graph(g),
      n(graph->getOrder()),
      color(c) {
    m = 0;
    for (uint a = 0; a < n; a++) m += g->getNodeDegree(a);
}

ExtendedSegmentStack::~ExtendedSegmentStack() {
    delete[] trailers;
    delete table;
    delete edges;
    delete[] big;
}

unsigned ExtendedSegmentStack::approximateEdge(uint u, uint k) {
    double g = ceil(graph->getNodeDegree(u) / static_cast<double>(l));
    unsigned f = static_cast<unsigned>(floor((k - 1) / g));
    return f;
}

void ExtendedSegmentStack::storeEdges() {
    for (uint c = 0; c < lp; c++) {
        uint u = low[c].head(), k = low[c].tail();
        if (graph->getNodeDegree(u) > m / q) {
            if (trailers[tp].bi == INVALID) {
                trailers[tp].bi = bp;
                trailers[tp].bc = 0;
            }
            big[bp++] = Pair(u, k - 1);  // another big vertex is stored
            if (bp > q) throw std::out_of_range("big storage is full!");
        } else {  // store an approximation
            unsigned f = approximateEdge(u, k);
            edges->insert(u, f);
        }
    }
}

int ExtendedSegmentStack::push(Pair p) {
    uint pu = p.head();
    if (lp < q) {
        table->insert(pu, tp);
        low[lp++] = p;
    } else if (hp < q) {
        table->insert(pu, tp + 1);
        high[hp++] = p;
    } else {
        trailers[tp].x = low[lp - 1];
        storeEdges();

        tp++;
        Pair *tmp = low;
        low = high;
        high = tmp;
        hp = 0;
        high[hp++] = p;
        table->insert(pu, tp + 1);
    }
    return 0;
}

bool ExtendedSegmentStack::isInTopSegment(uint u, bool restoring) {
    bool r = false;
    unsigned top = hp > 0 ? tp + 1 : lp > 0 ? tp : tp - 1;
    if (restoring && lp > 0) top--;
    r = table->get(u) == top;
    return r;
}

uint ExtendedSegmentStack::retrieveEdge(uint u, unsigned f) {
    unsigned g = static_cast<unsigned>(
        ceil(graph->getNodeDegree(u) / static_cast<double>(l)));
    return f * g;
}

uint ExtendedSegmentStack::getOutgoingEdge(uint u) {
    if (graph->getNodeDegree(u) > m / q) {
        if (tp > 0) {  // tp>0 should be given in every restoration, which is
                       // precisely when this method may be called
            Trailer *t = trailers + (tp - 1);
            Pair x = big[t->bi + t->bc];
            t->bc += 1;
            return x.tail();
        } else {
            throw E_noTrailers();
        }
    } else {
        return retrieveEdge(u, edges->get(u));
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

int ExtendedSegmentStack::getTopTrailer(Pair *r) {
    if (tp > 0) {
        *r = trailers[tp - 1].x;
        return 0;
    } else {
        return 1;
    }
}

void ExtendedSegmentStack::recolorLow(unsigned v) {
    for (unsigned a = 0; a < q; a++) color->insert(low[a].head(), v);
}

bool ExtendedSegmentStack::isAligned() {
    bool r = false;
    if (lp == q && tp > 0) {
        Trailer t = trailers[tp - 1];
        r = low[lp - 1] == t.x;
        if (r) {
            if (tp > 0) trailers[tp - 1].bi = INVALID;
            tp--;
        }
    }
    return r;
}

Pair ExtendedSegmentStack::top() const {
    Pair r =
        hp > 0 ? high[hp - 1] : lp > 0 ? low[lp - 1] : Pair(INVALID, INVALID);
    return r;
}

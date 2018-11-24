#include "sealib/segmentstack.h"
#include <math.h>
#include <sstream>
#include <stack>
#include <stdexcept>

using Sealib::SegmentStack;
using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;

//  -- SUPERCLASS --

SegmentStack::SegmentStack(uint32_t segmentSize)
    : q(segmentSize), low(q), high(q), lp(0), hp(0), tp(0) {}

int SegmentStack::pop(std::pair<uint, uint> *r) {
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

BasicSegmentStack::BasicSegmentStack(uint32_t segmentSize)
    : SegmentStack(segmentSize) {}

void BasicSegmentStack::push(std::pair<uint, uint> u) {
    if (lp < q) {
        low[lp++] = u;
    } else if (hp < q) {
        high[hp++] = u;
    } else {
        last = low[lp - 1];
        tp++;
        std::swap(low, high);
        hp = 0;
        high[hp++] = u;
    }
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
        unsigned lu = low[lp - 1].first, lk = low[lp - 1].second;
        unsigned hu = 0, hk = 0;
        if (alignTarget == 2) hu = high[hp - 1].first, hk = high[hp - 1].second;
        unsigned tu = savedTrailer.first, tk = savedTrailer.second;
        r = (alignTarget == 2 && hu == tu && hk == tk) ||
            (alignTarget == 1 && lu == tu && lk == tk);
    }
    return r;
}

//  -- EXTENDED --

ExtendedSegmentStack::ExtendedSegmentStack(uint size, Graph *g, CompactArray *c)
    : SegmentStack(static_cast<uint32_t>(ceil(size / log2(size)))),
      trailers(size / q + 1),
      l(static_cast<uint32_t>(ceil(log2(size))) + 1),
      table(size, l),
      edges(size, l),
      big(q),
      bp(0),
      graph(g),
      n(graph->getOrder()),
      color(c) {
    m = 0;
    for (uint a = 0; a < n; a++) m += g->getNodeDegree(a);
}

uint32_t ExtendedSegmentStack::approximateEdge(uint u, uint k) {
    double g = ceil(graph->getNodeDegree(u) / static_cast<double>(l));
    uint32_t f = static_cast<uint32_t>(floor((k - 1) / g));
    return f;
}

void ExtendedSegmentStack::storeEdges() {
    for (uint c = 0; c < lp; c++) {
        uint u = low[c].first, k = low[c].second;
        if (graph->getNodeDegree(u) > m / q) {
            if (trailers[tp].bi == INVALID) {
                trailers[tp].bi = bp;
                trailers[tp].bc = 0;
            }
            big[bp++] = std::pair<uint, uint>(
                u, k - 1);  // another big vertex is stored
            if (bp > q) throw std::out_of_range("big storage is full!");
        } else {  // store an approximation
            uint32_t f = approximateEdge(u, k);
            edges.insert(u, f);
        }
    }
}

void ExtendedSegmentStack::push(std::pair<uint, uint> p) {
    uint pu = p.first;
    if (lp < q) {
        table.insert(pu, tp);
        low[lp++] = p;
    } else if (hp < q) {
        table.insert(pu, tp + 1);
        high[hp++] = p;
    } else {
        trailers[tp].x = low[lp - 1];
        storeEdges();

        tp++;
        std::swap(low, high);
        hp = 0;
        high[hp++] = p;
        table.insert(pu, tp + 1);
    }
}

bool ExtendedSegmentStack::isInTopSegment(uint u, bool restoring) {
    bool r = false;
    uint32_t top = hp > 0 ? tp + 1 : lp > 0 ? tp : tp - 1;
    if (restoring && lp > 0) top--;
    r = table.get(u) == top;
    return r;
}

uint ExtendedSegmentStack::retrieveEdge(uint u, uint32_t f) {
    uint32_t g = static_cast<uint32_t>(
        ceil(graph->getNodeDegree(u) / static_cast<double>(l)));
    return f * g;
}

uint ExtendedSegmentStack::getOutgoingEdge(uint u) {
    if (graph->getNodeDegree(u) > m / q) {
        if (tp > 0) {  // tp>0 should be given in every restoration, which is
                       // precisely when this method may be called
            Trailer &t = trailers[tp - 1];
            std::pair<uint, uint> x = big[t.bi + t.bc];
            t.bc += 1;
            return x.second;
        } else {
            throw std::logic_error(
                "can't get edge from big vertex because there are no trailers");
        }
    } else {
        return retrieveEdge(u, edges.get(u));
    }
}

int ExtendedSegmentStack::getRestoreTrailer(std::pair<uint, uint> *r) {
    if (tp > 1) {
        *r = trailers[tp - 2].x;
        return 0;
    } else {
        return 1;
    }
}

int ExtendedSegmentStack::getTopTrailer(std::pair<uint, uint> *r) {
    if (tp > 0) {
        *r = trailers[tp - 1].x;
        return 0;
    } else {
        return 1;
    }
}

void ExtendedSegmentStack::recolorLow(unsigned v) {
    for (unsigned a = 0; a < q; a++) color->insert(low[a].first, v);
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

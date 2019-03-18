#include "sealib/collection/segmentstack.h"
#include <math.h>
#include <sstream>
#include <stack>
#include "sealib/iterator/dfs.h"

namespace Sealib {

//  -- SUPERCLASS --

SegmentStack::SegmentStack(uint64_t segmentSize)
    : q(segmentSize), low(q), high(q), lp(0), hp(0), tp(0) {}

uint8_t SegmentStack::pop(std::pair<uint64_t, uint64_t> *r) {
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

std::pair<uint64_t, uint64_t> SegmentStack::top() {
    if (hp > 0) {
        return high[hp - 1];
    } else if (lp > 0) {
        return low[lp - 1];
    } else {
        return std::pair<uint64_t, uint64_t>(INVALID, tp);
    }
}

bool SegmentStack::isEmpty() {
    bool r;
    r = lp == 0 && hp == 0 && tp == 0;
    return r;
}

uint64_t SegmentStack::size() { return tp * q + hp + lp; }

//  -- BASIC --

BasicSegmentStack::BasicSegmentStack(uint64_t segmentSize)
    : SegmentStack(segmentSize) {}

void BasicSegmentStack::push(std::pair<uint64_t, uint64_t> u) {
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
        throw TrailersEmpty();
    }
}

bool BasicSegmentStack::isAligned() {
    bool r = false;
    if ((alignTarget == 2 && hp < q) || lp < q) {
        r = false;
    } else {
        uint64_t lu = low[lp - 1].first, lk = low[lp - 1].second;
        uint64_t hu = 0, hk = 0;
        if (alignTarget == 2) hu = high[hp - 1].first, hk = high[hp - 1].second;
        uint64_t tu = savedTrailer.first, tk = savedTrailer.second;
        r = (alignTarget == 2 && hu == tu && hk == tk) ||
            (alignTarget == 1 && lu == tu && lk == tk);
    }
    return r;
}

//  -- EXTENDED --

ExtendedSegmentStack::ExtendedSegmentStack(uint64_t size, Graph const &g,
                                           CompactArray *c)
    : SegmentStack(static_cast<uint64_t>(ceil(size / log2(size)))),
      trailers(size / q + 1),
      l(static_cast<uint64_t>(ceil(log2(size))) + 1),
      table(size, l),
      edges(size, l),
      big(q),
      bp(0),
      graph(g),
      n(graph.getOrder()),
      color(c) {
    m = 0;
    for (uint64_t a = 0; a < n; a++) m += g.deg(a);
}

uint64_t ExtendedSegmentStack::approximateEdge(uint64_t u, uint64_t k) {
    double g = ceil(graph.deg(u) / static_cast<double>(l));
    uint64_t f = static_cast<uint64_t>(floor((k - 1) / g));
    return f;
}

void ExtendedSegmentStack::storeEdges() {
    for (uint64_t c = 0; c < lp; c++) {
        uint64_t u = low[c].first, k = low[c].second;
        if (graph.deg(u) > m / q) {
            if (trailers[tp].bi == INVALID) {
                trailers[tp].bi = bp;
                trailers[tp].bc = 0;
            }
            big[bp++] = std::pair<uint64_t, uint64_t>(
                u, k - 1);  // another big vertex is stored
            if (bp > q) throw BigStackFull();
        } else {  // store an approximation
            uint64_t f = approximateEdge(u, k);
            edges.insert(u, f);
        }
    }
}

void ExtendedSegmentStack::push(std::pair<uint64_t, uint64_t> p) {
    uint64_t pu = p.first;
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

bool ExtendedSegmentStack::isInTopSegment(uint64_t u, bool restoring) {
    bool r = false;
    uint64_t top = hp > 0 ? tp + 1 : lp > 0 ? tp : tp - 1;
    if (restoring && lp > 0) top--;
    r = table.get(u) == top;
    return r;
}

uint64_t ExtendedSegmentStack::retrieveEdge(uint64_t u, uint64_t f) {
    uint64_t g =
        static_cast<uint64_t>(ceil(graph.deg(u) / static_cast<double>(l)));
    return f * g;
}

uint64_t ExtendedSegmentStack::getOutgoingEdge(uint64_t u) {
    if (graph.deg(u) > m / q) {
        if (tp > 0) {  // tp>0 should be given in every restoration, which is
                       // precisely when this method may be called
            Trailer &t = trailers[tp - 1];
            std::pair<uint64_t, uint64_t> x = big[t.bi + t.bc];
            t.bc += 1;
            return x.second;
        } else {
            throw TrailersEmpty();
        }
    } else {
        return retrieveEdge(u, edges.get(u));
    }
}

int ExtendedSegmentStack::getRestoreTrailer(std::pair<uint64_t, uint64_t> *r) {
    if (tp > 1) {
        *r = trailers[tp - 2].x;
        return 0;
    } else {
        return 1;
    }
}

int ExtendedSegmentStack::getTopTrailer(std::pair<uint64_t, uint64_t> *r) {
    if (tp > 0) {
        *r = trailers[tp - 1].x;
        return 0;
    } else {
        return 1;
    }
}

void ExtendedSegmentStack::recolorLow(uint64_t v) {
    for (uint64_t a = 0; a < q; a++) color->insert(low[a].first, v);
}

std::pair<bool, uint64_t> ExtendedSegmentStack::findEdge(uint64_t u,
                                                         uint64_t k) {
    std::pair<bool, uint64_t> r = {false, INVALID};
    for (uint64_t i = k; i < graph.deg(u); i++) {
        uint64_t v = graph.head(u, i);
        if (color->get(v) == DFS_GRAY && isInTopSegment(v, true)) {
            r = {true, i};
            break;
        }
    }
    if (!r.first) {
        std::pair<uint64_t, uint64_t> a;
        getTopTrailer(&a);
        r = {false, a.second - 1};
    }
    return r;
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

}  // namespace Sealib

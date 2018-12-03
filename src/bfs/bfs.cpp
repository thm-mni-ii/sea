#include "sealib/iterator/bfs.h"
#include <stdexcept>
#include <vector>

using Sealib::BFS;
using Sealib::CompactArray;
using Sealib::Graph;

void BFS::init() {
    u = 0;
    dist = 0;
    innerGray = BFS_GRAY1;
    outerGray = BFS_GRAY2;
    preprocess(0);
    color.insert(0, innerGray);
}

bool BFS::nextComponent() {
    bool found = false;
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == BFS_WHITE) {
            u = a;
            found = true;
            dist = 0;
            preprocess(u);
            color.insert(u, innerGray);
            break;
        }
    }
    return found;
}

bool BFS::hasGrayNode() {
    // needs 4-color CD:
    // try {
    //   c.choice();
    //   return true;
    // } catch(std::exception) {
    //   return false;
    // }

    // TEMPORARY solution:
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == BFS_GRAY1 || color.get(a) == BFS_GRAY2) return true;
    }
    return false;
}
uint BFS::getGrayNode() {
    // return choice();

    // TEMPORARY solution:
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == innerGray) {
            return a;
        }
    }
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == outerGray) {
            return a;
        }
    }
    throw std::logic_error(
        "BFS: no more gray nodes found; did you forget to call "
        "nextComponent()?");
}

bool BFS::more() { return hasGrayNode(); }

std::pair<uint, uint> BFS::next() {
    u = getGrayNode();
    if (color.get(u) == outerGray) {
        uint32_t tmp = innerGray;
        innerGray = outerGray;
        outerGray = tmp;
        dist++;
    }
    for (uint k = 0; k < g->getNodeDegree(u); k++) {
        uint v = g->head(u, k);
        preexplore(u, v);
        if (color.get(v) == BFS_WHITE) {
            preprocess(v);
            color.insert(v, outerGray);
        }
    }
    color.insert(u, BFS_BLACK);
    return std::pair<uint, uint>(u, dist);
}

BFS::BFS(Graph *graph, Consumer pp, BiConsumer pe)
    : g(graph), n(g->getOrder()), color(n, 4), preprocess(pp), preexplore(pe) {
    for (uint a = 0; a < n; a++) color.insert(a, BFS_WHITE);
}

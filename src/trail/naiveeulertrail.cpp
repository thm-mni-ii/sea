#include "./naiveeulertrail.h"
#include "./naivetrailstructure.h"
#include <map>
#include <set>
#include <tuple>

using Sealib::NaiveEulerTrail;
using Sealib::UndirectedGraph;
using std::shared_ptr;
using std::vector;
using std::list;
using std::map;
using std::set;
using std::tuple;

NaiveEulerTrail::NaiveEulerTrail(const shared_ptr<UndirectedGraph> &g) {
    static const std::tuple<uint64_t, uint64_t> nonArc =
        std::make_tuple(INVALID, INVALID);

    uint64_t order = g->getOrder();
    std::vector<Sealib::NaiveTrailStructure *> ts;
    for (uint64_t i = 0; i < order; i++) {
        uint64_t degree = g->getNode(i).getDegree();
        ts.push_back(new Sealib::NaiveTrailStructure(degree));
    }

    // find first start node
    uint64_t u = INVALID;
    for (uint64_t i = 0; i < order; i++) {
        if (g->getNode(i).getDegree() % 2 != 0) {  // odd
            u = i;
            break;
        }
    }
    if (u == INVALID) {  // no odd found
        for (uint64_t i = 0; i < order; i++) {
            // first that has edges, it's possible to have a graph with no edges
            if (g->getNode(i).getDegree() != 0) {
                u = i;
                break;
            }
        }
    }
    if (u != INVALID) {
        trails.emplace_back();
    }

    // loop the iteration while there is a non-black vertex
    while (u != INVALID) {
        std::tuple<uint64_t, uint64_t> aOld = nonArc;
        auto tOld = trails.begin();
        if (ts[u]->isEven() && ts[u]->isGrey()) {  // remember aOld
            while (tOld++ != trails.end()) {
                aOld = (*tOld).getOutgoingFrom(u);
                if (aOld != nonArc) break;
            }
        }
        Sealib::SimpleTrail tempTrail;
        uint64_t k = ts[u]->leave();
        uint64_t uMate;
        do {
            uint64_t from = u;
            uMate = g->getNode(u).getAdj()[k].second;
            u = g->getNode(u).getAdj()[k].first;  // next node
            k = ts[u]->enter(uMate);
            tempTrail.addArc(std::make_tuple(from, u));
        } while (k != INVALID);

        if (aOld != nonArc) {
            uint64_t idx = (*tOld).getFirstIndexOf(aOld);
            (*tOld).insertSubTrail(tempTrail, idx);
        } else {
            trails.insert(trails.end(), tempTrail);
        }

        // find next start node
        u = INVALID;

        for (uint64_t i = 0; i < order; i++) {
            if (!ts[i]->isEven() && !ts[i]->isBlack()) {  // odd
                u = i;
                break;
            }
        }
        if (u == INVALID) {  // no odd found, search for grey
            for (uint64_t i = 0; i < order; i++) {
                // first that has edges, it's possible to have a graph with no edges
                if (ts[i]->isGrey()
                    && !ts[i]->isBlack()) {
                    u = i;
                    break;
                }
            }
        }
        if (u == INVALID) {  // no odd found and no grey found, search for white
            for (uint64_t i = 0; i < order; i++) {
                // first that has edges, it's possible to have a graph with no edges
                if (!ts[i]->isBlack()) {
                    u = i;
                    break;
                }
            }
        }
        // no node left, should break now.
    }
}

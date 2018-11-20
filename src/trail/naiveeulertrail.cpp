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
    static const std::tuple<uint32_t, uint32_t> nonArc =
        std::make_tuple((uint32_t) -1, (uint32_t) -1);

    uint32_t order = g->getOrder();
    std::vector<Sealib::NaiveTrailStructure *> ts;
    for (uint32_t i = 0; i < order; i++) {
        uint32_t degree = g->getNode(i).getDegree();
        ts.push_back(new Sealib::NaiveTrailStructure(degree));
    }

    // find first start node
    uint32_t u = (uint32_t) -1;
    for (uint32_t i = 0; i < order; i++) {
        if (g->getNode(i).getDegree() % 2 != 0) {  // odd
            u = i;
            break;
        }
    }
    if (u == (uint32_t) -1) {  // no odd found
        for (uint32_t i = 0; i < order; i++) {
            // first that has edges, it's possible to have a graph with no edges
            if (g->getNode(i).getDegree() != 0) {
                u = i;
                break;
            }
        }
    }
    if (u != (uint32_t) -1) {
        trails.emplace_back();
    }

    // loop the iteration while there is a non-black vertex
    while (u != (uint32_t) -1) {
        std::tuple<uint32_t, uint32_t> aOld = nonArc;
        auto tOld = trails.begin();
        if (ts[u]->isEven() && ts[u]->isGrey()) {  // remember aOld
            while (tOld++ != trails.end()) {
                aOld = (*tOld).getOutgoingFrom(u);
                if (aOld != nonArc) break;
            }
        }
        Sealib::SimpleTrail tempTrail;
        uint32_t k = ts[u]->leave();
        uint32_t uMate;
        do {
            uint32_t from = u;
            uMate = g->getNode(u).getAdj()[k].crossIndex;
            u = g->getNode(u).getAdj()[k].vertex;  // next node
            k = ts[u]->enter(uMate);
            tempTrail.addArc(std::make_tuple(from, u));
        } while (k != (uint32_t) -1);

        if (aOld != nonArc) {
            uint64_t idx = (*tOld).getFirstIndexOf(aOld);
            (*tOld).insertSubTrail(tempTrail, idx);
        } else {
            trails.insert(trails.end(), tempTrail);
        }

        // find next start node
        u = (uint32_t) -1;

        for (uint32_t i = 0; i < order; i++) {
            if (!ts[i]->isEven() && !ts[i]->isBlack()) {  // odd
                u = i;
                break;
            }
        }
        if (u == (uint32_t) -1) {  // no odd found, search for grey
            for (uint32_t i = 0; i < order; i++) {
                // first that has edges, it's possible to have a graph with no edges
                if (ts[i]->isGrey()
                    && !ts[i]->isBlack()) {
                    u = i;
                    break;
                }
            }
        }
        if (u == (uint32_t) -1) {  // no odd found and no grey found, search for white
            for (uint32_t i = 0; i < order; i++) {
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

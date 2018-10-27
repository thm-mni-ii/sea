#include <src/naiveeulertrail.h>
#include <src/simpletrail.h>
#include <src/naivetrailstructure.h>
#include <map>
#include <set>
#include <tuple>

using Sealib::NaiveEulerTrail;
using Sealib::BasicGraph;
using std::shared_ptr;
using std::vector;
using std::list;
using std::map;
using std::set;
using std::tuple;

NaiveEulerTrail::NaiveEulerTrail(const shared_ptr<BasicGraph> &g) {
    static const std::tuple<unsigned int, unsigned int> nonArc =
        std::make_tuple((unsigned int) -1, (unsigned int) -1);

    unsigned int order = g->getOrder();
    std::vector<Sealib::NaiveTrailStructure *> ts;
    for (unsigned int i = 0; i < order; i++) {
        unsigned int degree = g->getNode(i).getDegree();
        ts.push_back(new Sealib::NaiveTrailStructure(degree));
    }

    // find first start node
    unsigned int u = (unsigned int) -1;
    for (unsigned int i = 0; i < order; i++) {
        if (g->getNode(i).getDegree() % 2 != 0) {  // odd
            u = i;
            break;
        }
    }
    if (u == (unsigned int) -1) {  // no odd found
        for (unsigned int i = 0; i < order; i++) {
            // first that has edges, it's possible to have a graph with no edges
            if (g->getNode(i).getDegree() != 0) {
                u = i;
                break;
            }
        }
    }
    if (u != (unsigned int) -1) {
        trails.emplace_back();
    }

    // loop the iteration while there is a non-black vertex
    while (u != (unsigned int) -1) {
        std::tuple<unsigned int, unsigned int> aOld = nonArc;
        auto tOld = trails.begin();
        if (ts[u]->isEven() && ts[u]->isGrey()) {  // remember aOld
            while (tOld++ != trails.end()) {
                aOld = (*tOld).getOutgoingFrom(u);
                if (aOld != nonArc) break;
            }
        }
        Sealib::SimpleTrail tempTrail;
        unsigned int k = ts[u]->leave();
        unsigned int uMate;
        do {
            unsigned int from = u;
            uMate = g->getNode(u).getAdj()[k].crossIndex;
            u = g->getNode(u).getAdj()[k].vertex;  // next node
            k = ts[u]->enter(uMate);
            tempTrail.addArc(std::make_tuple(from, u));
        } while (k != (unsigned int) -1);

        if (aOld != nonArc) {
            unsigned long idx = (*tOld).getFirstIndexOf(aOld);
            (*tOld).insertSubTrail(tempTrail, idx);
        } else {
            trails.insert(trails.end(), tempTrail);
        }

        // find next start node
        u = (unsigned int) -1;

        for (unsigned int i = 0; i < order; i++) {
            if (!ts[i]->isEven() && !ts[i]->isBlack()) {  // odd
                u = i;
                break;
            }
        }
        if (u == (unsigned int) -1) {  // no odd found, search for grey
            for (unsigned int i = 0; i < order; i++) {
                // first that has edges, it's possible to have a graph with no edges
                if (ts[i]->isGrey()
                    && !ts[i]->isBlack()) {
                    u = i;
                    break;
                }
            }
        }
        if (u == (unsigned int) -1) {  // no odd found and no grey found, search for white
            for (unsigned int i = 0; i < order; i++) {
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

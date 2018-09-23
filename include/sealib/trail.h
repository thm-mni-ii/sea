#ifndef SEALIB_TRAIL_H_
#define SEALIB_TRAIL_H_

#include <sealib/rankselect.h>
#include <sealib/basicgraph.h>
#include <sealib/simpletrailstructure.h>
#include <sealib/trailstructure.h>
#include <ostream>
#include <vector>
#include <tuple>

namespace Sealib {
template<class TrailStructureType>
class EulerTrail {
    typedef typename Sealib::RankSelect rankselect_t;
    typedef typename Sealib::BasicGraph graph_t;
    typedef typename std::shared_ptr<graph_t> graphptr_t;
    typedef typename Sealib::Bitset<unsigned char> bitset_t;
    typedef typename std::vector<TrailStructureType> array_t;

 private:
    graphptr_t graph;
    array_t trail;
    rankselect_t trailStarts;

    unsigned int findStartingNode();
    array_t initializeTrail();
    bitset_t findTrailStarts();

    class iterator {
     public:
        explicit iterator(const EulerTrail<TrailStructureType> &Container, unsigned int index = 1);
        std::tuple<unsigned long, bool> operator*() const;
        iterator &operator++();
        iterator &operator++(int);
        bool operator!=(const iterator &) const;
     private:
        unsigned int nIndex;
        unsigned int mIndex;
        unsigned int arc;
        bool ending;
        const EulerTrail<TrailStructureType> &eulerTrail;
    };

 public:
    explicit EulerTrail(const graphptr_t &graph);

    friend std::ostream &operator<<(std::ostream &os,
                                    const EulerTrail<TrailStructureType> &eulerTrail) {
        unsigned int tourNum = 1;
        bool newTour = true;
        for (auto v0 : eulerTrail) {
            bool ending = std::get<1>(v0);
            if (!ending) {
                if (newTour) {
                    newTour = false;
                    os << "T" << tourNum++ << ": ";
                }
                os << std::get<0>(v0) << " -> ";
            }
            if (ending) {
                newTour = true;
                os << std::get<0>(v0) << std::endl;
            }
        }
        return os;
    }
    iterator begin() const;
    iterator end() const;
};
}  // namespace Sealib
#endif  // SEALIB_TRAIL_H_

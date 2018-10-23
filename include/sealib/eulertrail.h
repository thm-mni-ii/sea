#ifndef SEALIB_EULERTRAIL_H_
#define SEALIB_EULERTRAIL_H_

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
 private:
    std::shared_ptr<Sealib::BasicGraph> graph;
    std::vector<TrailStructureType>  trail;
    Sealib::RankSelect trailStarts;

    unsigned int findStartingNode();
    std::vector<TrailStructureType> initializeTrail();
    Sealib::Bitset<unsigned char> findTrailStarts();

    class iterator {
     public:
        explicit iterator(const EulerTrail<TrailStructureType> &Container, unsigned int index = 1);
        std::tuple<unsigned long, bool> operator*() const;
        iterator &operator++();
        iterator &operator++(int);
        bool operator!=(const iterator &) const;
     private:
        const EulerTrail<TrailStructureType> &eulerTrail;
        unsigned int nIndex;
        unsigned int mIndex;
        unsigned int arc;
        bool ending;
    };

 public:
    explicit EulerTrail(const std::shared_ptr<Sealib::BasicGraph> &graph);

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
#endif  // SEALIB_EULERTRAIL_H_

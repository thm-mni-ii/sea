#ifndef SEALIB_NAIVETRAIL_H_
#define SEALIB_NAIVETRAIL_H_

#include <sealib/basicgraph.h>
#include <memory>
#include <ostream>
#include <vector>
#include <tuple>
#include <list>

namespace Sealib {
class NaiveTrail {
    typedef typename Sealib::BasicGraph graph_t;
    typedef typename std::shared_ptr<graph_t> graphptr_t;
    typedef typename std::list<unsigned long> list_t;
    typedef typename std::vector<list_t> trail_container_t;

 private:
    graphptr_t graph;
    trail_container_t trails;

    class iterator {
     public:
        explicit iterator(const NaiveTrail &Container, unsigned int index = 1);
        std::tuple<unsigned long, bool> operator*() const;
        iterator &operator++();
        iterator &operator++(int);
        bool operator!=(const iterator &) const;
     private:
        const NaiveTrail &naiveTrail;
        unsigned int nIndex;
        unsigned int mIndex;
        unsigned int arc;
        bool ending;
    };

 public:
    explicit NaiveTrail(const graphptr_t &graph);

    friend std::ostream &operator<<(std::ostream &os,
                                    const NaiveTrail &naiveTrail) {
        unsigned int tourNum = 1;
        bool newTour = true;
        for (auto v0 : naiveTrail) {
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
#endif  // SEALIB_NAIVETRAIL_H_

//
// Created by jmeintrup on 27.05.18.
//

#ifndef SEA_SIMPLETRAIL_H
#define SEA_SIMPLETRAIL_H

#include <vector>
#include <tuple>

namespace Sealib {
    class SimpleTrail {
    private:
        std::vector<std::tuple<unsigned int, unsigned int>> trail;
    public:
        SimpleTrail();
        SimpleTrail(unsigned int toReserve);
        void insertSubTrail(const SimpleTrail &subTrail, unsigned long idx);
        void pushBackSubTrail(const SimpleTrail &subTrail);
        void addArc(std::tuple<unsigned int, unsigned int> arc);
        unsigned long getFirstIndexOf(std::tuple<unsigned int, unsigned int> arc) const;
        const std::vector<std::tuple<unsigned int, unsigned int>>& getTrail()  const;

        std::tuple<unsigned int, unsigned int> getOutgoingFrom(unsigned int u) const;
    };
}
#endif //SEA_SIMPLETRAIL_H

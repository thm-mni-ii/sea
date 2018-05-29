//
// Created by jmeintrup on 27.05.18.
//

#ifndef SEA_SIMPLETRAIL_H
#define SEA_SIMPLETRAIL_H

#include <vector>
#include <list>

namespace Sealib {
    class SimpleTrail {
    private:
        std::vector<unsigned int> trail;
    public:
        SimpleTrail();
        SimpleTrail(unsigned int toReserve);
        void insertSubTrail(const SimpleTrail &subTrail, unsigned int idx);
        void addEdge(unsigned int edge);
        const std::vector<unsigned int>& getTrail()  const;
    };
}
#endif //SEA_SIMPLETRAIL_H

#ifndef SEALIB_NAIVETRAIL_H_
#define SEALIB_NAIVETRAIL_H_

#include <vector>
#include <tuple>

namespace Sealib {
class NaiveTrail {
 private:
    std::vector<std::tuple<unsigned int, unsigned int>> trail;

 public:
    NaiveTrail();
    NaiveTrail(unsigned int toReserve);
    void insertSubTrail(const NaiveTrail &subTrail, unsigned long idx);
    void pushBackSubTrail(const NaiveTrail &subTrail);
    void addArc(std::tuple<unsigned int, unsigned int> arc);
    unsigned long getFirstIndexOf(std::tuple<unsigned int, unsigned int> arc) const;
    const std::vector<std::tuple<unsigned int, unsigned int>>& getTrail()  const;

    std::tuple<unsigned int, unsigned int> getOutgoingFrom(unsigned int u) const;

};
}  // namespace Sealib
#endif //SEALIB_NAIVETRAIL_H_

//
// Created by jmeintrup on 27.05.18.
//

#ifndef SEA_SIMPLETRAIL_H
#define SEA_SIMPLETRAIL_H



namespace Sealib {
class SimpleTrail {


};
}
#endif //SEA_SIMPLETRAIL_H
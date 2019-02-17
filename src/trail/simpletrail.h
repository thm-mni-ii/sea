#ifndef SRC_TRAIL_SIMPLETRAIL_H_
#define SRC_TRAIL_SIMPLETRAIL_H_

#include <vector>
#include <tuple>
#include "sealib/_types.h"

namespace Sealib {
class SimpleTrail {
 private:
    std::vector<std::tuple<uint64_t, uint64_t>> trail;
 public:
    SimpleTrail();
    void insertSubTrail(const SimpleTrail &subTrail, uint64_t idx);
    void pushBackSubTrail(const SimpleTrail &subTrail);
    void addArc(std::tuple<uint64_t, uint64_t> arc);
    uint64_t getFirstIndexOf(std::tuple<uint64_t, uint64_t> arc) const;
    const std::vector<std::tuple<uint64_t, uint64_t>> &getTrail() const;

    std::tuple<uint64_t, uint64_t> getOutgoingFrom(uint64_t u) const;
};
}  // namespace Sealib
#endif  // SRC_TRAIL_SIMPLETRAIL_H_

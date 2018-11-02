#ifndef SRC_TRAIL_SIMPLETRAIL_H_
#define SRC_TRAIL_SIMPLETRAIL_H_

#include <vector>
#include <tuple>

namespace Sealib {
class SimpleTrail {
 private:
    std::vector<std::tuple<uint32_t, uint32_t>> trail;
 public:
    SimpleTrail();
    void insertSubTrail(const SimpleTrail &subTrail, uint64_t idx);
    void pushBackSubTrail(const SimpleTrail &subTrail);
    void addArc(std::tuple<uint32_t, uint32_t> arc);
    uint64_t getFirstIndexOf(std::tuple<uint32_t, uint32_t> arc) const;
    const std::vector<std::tuple<uint32_t, uint32_t>> &getTrail() const;

    std::tuple<uint32_t, uint32_t> getOutgoingFrom(uint32_t u) const;
};
}  // namespace Sealib
#endif  // SRC_TRAIL_SIMPLETRAIL_H_

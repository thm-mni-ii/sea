#ifndef SRC_SIMPLETRAIL_H_
#define SRC_SIMPLETRAIL_H_

#include <vector>
#include <tuple>

namespace Sealib {
class SimpleTrail {
 private:
    std::vector<std::tuple<unsigned int, unsigned int>> trail;
 public:
    SimpleTrail();
    void insertSubTrail(const SimpleTrail &subTrail, unsigned long idx);
    void pushBackSubTrail(const SimpleTrail &subTrail);
    void addArc(std::tuple<unsigned int, unsigned int> arc);
    unsigned long getFirstIndexOf(std::tuple<unsigned int, unsigned int> arc) const;
    const std::vector<std::tuple<unsigned int, unsigned int>> &getTrail() const;

    std::tuple<unsigned int, unsigned int> getOutgoingFrom(unsigned int u) const;
};
}  // namespace Sealib
#endif  // SRC_SIMPLETRAIL_H_

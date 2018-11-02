#ifndef SRC_TRAIL_NAIVETRAILSTRUCTURE_H_
#define SRC_TRAIL_NAIVETRAILSTRUCTURE_H_

#include <map>
#include <list>

namespace Sealib {
class NaiveTrailStructure {
 private:
    std::map<uint32_t, uint32_t> pairedEdges;
    std::list<uint32_t> unusedEdges;
    uint32_t currentDegree;
    bool black;
    bool grey;
 public:
    explicit NaiveTrailStructure(uint32_t degree_);
    uint32_t leave();
    uint32_t enter(uint32_t e);
    bool isBlack();
    bool isWhite();
    bool isGrey();
    bool isEven();
    uint32_t getMatched(uint32_t i);
};
}  // namespace Sealib
#endif  // SRC_TRAIL_NAIVETRAILSTRUCTURE_H_

#ifndef SRC_TRAIL_NAIVETRAILSTRUCTURE_H_
#define SRC_TRAIL_NAIVETRAILSTRUCTURE_H_

#include <map>
#include <list>

namespace Sealib {
class NaiveTrailStructure {
 private:
    std::map<uint64_t, uint64_t> pairedEdges;
    std::list<uint64_t> unusedEdges;
    uint64_t currentDegree;
    bool black;
    bool grey;
 public:
    explicit NaiveTrailStructure(uint64_t degree_);
    uint64_t leave();
    uint64_t enter(uint64_t e);
    bool isBlack();
    bool isWhite();
    bool isGrey();
    bool isEven();
    uint64_t getMatched(uint64_t i);
};
}  // namespace Sealib
#endif  // SRC_TRAIL_NAIVETRAILSTRUCTURE_H_

#ifndef SRC_NAIVETRAILSTRUCTURE_H_
#define SRC_NAIVETRAILSTRUCTURE_H_

#include <map>
#include <list>

namespace Sealib {
class NaiveTrailStructure {
 private:
    std::map<unsigned int, unsigned int> pairedEdges;
    std::list<unsigned int> unusedEdges;
    unsigned int currentDegree;
    bool black;
    bool grey;
 public:
    explicit NaiveTrailStructure(unsigned int degree_);
    unsigned int leave();
    unsigned int enter(unsigned int e);
    bool isBlack();
    bool isWhite();
    bool isGrey();
    bool isEven();
    unsigned int getMatched(unsigned int i);
};
}  // namespace Sealib
#endif  // SRC_NAIVETRAILSTRUCTURE_H_

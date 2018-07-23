#ifndef SEALIB_SHAREDRANKSELECT_H
#define SEALIB_SHAREDRANKSELECT_H

#include <sealib/sharedrankstructure.h>
#include <sealib/rankselect.h>
#include <sealib/bitset.h>

namespace Sealib{

 /**
 * Space efficient RankSelect implementation.
 * Extends the RankStructure with the select function.
 * @author Johannes Meintrup
 */
class SharedRankSelect : public RankSelect, public SharedRankStructure {
 private:
    SharedRankStructure *firstInSegment;

 public:

    /**
     * @param bitset for rankselect
     */
    explicit SharedRankSelect(std::shared_ptr<Bitset<unsigned char> >bitset);
    SharedRankSelect();

    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    unsigned long select(unsigned long k) const;
};
}  // namespace Sealib
#endif //SEALIB_RANKSELECT_H

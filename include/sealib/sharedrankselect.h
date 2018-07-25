#ifndef SEALIB_SHAREDRANKSELECT_H_
#define SEALIB_SHAREDRANKSELECT_H_

#include <sealib/sharedrankstructure.h>
#include <sealib/rankselect.h>
#include <sealib/bitset.h>

namespace Sealib {
/**
* Space efficient RankSelect implementation.
* Uses a shared_ptr for the bitset, saves space if the bitset can be reused.
* @author Johannes Meintrup
*/
class SharedRankSelect : public RankSelect, public SharedRankStructure {
 private:
    SharedRankStructure *firstInSegment;

 public:
    /**
     * @param shared_ptr managing the Sealib::Bitset used for RankSelect
     */
    explicit SharedRankSelect(std::shared_ptr<Bitset<unsigned char> > bitset);
    SharedRankSelect();

    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    unsigned long select(unsigned long k) const override;

    virtual ~SharedRankSelect();
};
}  // namespace Sealib
#endif  // SEALIB_SHAREDRANKSELECT_H_

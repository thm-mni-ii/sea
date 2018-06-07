//
// Created by jmeintrup on 10.05.18.
//

#ifndef SEA_RANKSELECT_H
#define SEA_RANKSELECT_H

#include "rankstructure.h"
namespace Sealib{

 /**
 * Space efficient RankSelect implementation.
 * Extends the RankStructure with the select function.
 * @author Johannes Meintrup
 */
class RankSelect : public RankStructure {
 private:
    std::unique_ptr<RankStructure> firstInSegment;

public:

    /**
     * @param bitset for rankselect
     */

    explicit RankSelect(std::unique_ptr<boost::dynamic_bitset<unsigned char>> bitsetPointer_);

    RankSelect();

    /**
     * Selects the k-th set bit
     * @param k idx
     * @return k-th set bit
     */
    unsigned long select(unsigned long k) const;
};
}  // namespace Sealib
#endif //SEA_RANKSELECT_H

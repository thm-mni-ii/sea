#ifndef SEALIB_RANKSELECT_H_
#define SEALIB_RANKSELECT_H_

#include <sealib/rankstructure.h>
namespace Sealib {
/**
 * RankStructure Interface to be used by the various RankStructure implementations.
 * Only provides a virtual function for rank.
 * @author Johannes Meintrup
 */
class RankSelect : public RankStructure {
 public:
    virtual ~RankSelect();
    virtual unsigned long select(unsigned long k) const = 0;
};
RankSelect::~RankSelect() = default;
}  // namespace Sealib
#endif  // SEALIB_RANKSELECT_H_

#ifndef SEALIB_RANKSTRUCTURE_H_
#define SEALIB_RANKSTRUCTURE_H_

/**
 * RankStructure Interface to be used by the various RankStructure implementations.
 * Only provides a virtual function for rank.
 * @author Johannes Meintrup
 */
namespace Sealib {
class RankStructure {
 public:
    /**
     * Rank of the k-th idx
     * @param k idx
     * @return rank of k-th idx
     */
    virtual unsigned long rank(unsigned long k) const = 0;
    virtual ~RankStructure() = default;
};
}  // namespace Sealib
#endif  // SEALIB_RANKSTRUCTURE_H_

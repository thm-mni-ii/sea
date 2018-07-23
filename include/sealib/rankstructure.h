#ifndef SEA_RANKSTRUCTURE_H
#define SEA_RANKSTRUCTURE_H
/**
 * Space efficient RankStructure implementation.
 * @author Johannes Meintrup
 */
namespace Sealib{
class RankStructure {
public:
 /**
  * Rank of the k-th idx
  * @param k idx
  * @return rank of k-th idx
  */
 virtual unsigned long rank(unsigned long k) const = 0;
};
}  // namespace Sealib
#endif //SEA_RANKSTRUCTURE_H

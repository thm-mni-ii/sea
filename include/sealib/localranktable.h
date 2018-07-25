#ifndef SEALIB_LOCALRANKTABLE_H_
#define SEALIB_LOCALRANKTABLE_H_

#include <vector>

namespace Sealib {
/**
* Used for RankSelect to lookup rank values in local segments of size 8-bits.
* All values are stored in a lookup table.
* @author Johannes Meintrup
*/
class LocalRankTable {
 private:
    std::vector<std::vector<unsigned char>> localRankLookupTable;
    /**
     * Used in getLocalRank for the first initialization of the table, never used again.
     */
    LocalRankTable();
    ~LocalRankTable();

    static LocalRankTable *instance;

 public:
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - unsinged char representing a bit vector of size 8
    * @param index in the segment
    * @return bits set up to and including the i-th bit
    */
    static unsigned char getLocalRank(unsigned char segment, unsigned char i);

    LocalRankTable(LocalRankTable const &) = delete;
    void operator=(LocalRankTable const &) = delete;
};
}  // namespace Sealib
#endif  // SEALIB_LOCALRANKTABLE_H_

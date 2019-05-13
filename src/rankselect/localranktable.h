#ifndef SRC_RANKSELECT_LOCALRANKTABLE_H_
#define SRC_RANKSELECT_LOCALRANKTABLE_H_

#include <cstdint>
#include <vector>

namespace Sealib {
/**
* Used for RankSelect to lookup rank values in local segments of size 8-bits.
* All values are stored in a lookup table.
* @author Johannes Meintrup
*/
class LocalRankTable {
 private:
    std::vector<std::vector<uint8_t>> localRankLookupTable;
    /**
     * Used in getLocalRank for the first initialization of the table, never used again.
     */
    LocalRankTable();
    ~LocalRankTable();

 public:
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment unsigned char representing a bit vector of size 8
    * @param i index in the segment
    * @return bits set up to and including the i-th bit
    */
    static uint8_t getLocalRank(uint8_t segment, uint8_t i);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalRankTable(LocalRankTable const &) = delete;
    void operator=(LocalRankTable const &) = delete;
};
}  // namespace Sealib
#endif  // SRC_RANKSELECT_LOCALRANKTABLE_H_

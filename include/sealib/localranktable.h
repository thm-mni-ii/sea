#ifndef SEALIB_LOCALRANKTABLE_H_
#define SEALIB_LOCALRANKTABLE_H_

#include <vector>

namespace Sealib {

template <typename BlockType = unsigned char>
class LocalRankTable;
}

namespace Sealib {
/**
* Used for RankSelect to lookup rank values in local segments of size 8-bits.
* All values are stored in a lookup table.
* @author Johannes Meintrup
*/
template <typename BlockType>
class LocalRankTable {

 private:
    std::vector<std::vector<BlockType>> localRankLookupTable;
    /**
     * Used in getLocalRank for the first initialization of the table, never used again.
     */
    LocalRankTable();
    ~LocalRankTable();

    static const constexpr unsigned int bitsPerBlock = sizeof(BlockType) * 8;
    static const constexpr BlockType blockTypeMax = (BlockType) - 1;

 public:
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - unsinged char representing a bit vector of size 8
    * @param index in the segment
    * @return bits set up to and including the i-th bit
    */
    static BlockType getLocalRank(BlockType segment, BlockType i);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalRankTable(LocalRankTable const &) = delete;
    void operator=(LocalRankTable const &) = delete;
};
}  // namespace Sealib
#endif  // SEALIB_LOCALRANKTABLE_H_

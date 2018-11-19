#ifndef SEALIB_LOCALSELECTTABLE_H_
#define SEALIB_LOCALSELECTTABLE_H_

#include <vector>
#include <limits>

namespace Sealib {

template <typename BlockType = unsigned char>
class LocalSelectTable;
}

namespace Sealib {
/**
* Used for RankSelect to lookup select values in local segments of size 8-bits.
* All values are stored in a lookup table.
* @author Johannes Meintrup
*/
template <typename BlockType>
class LocalSelectTable {
 private:
    std::vector<std::vector<BlockType>> localSelectLookupTable;
    /**
     * Used in getLocalSelect for the first initialization of the table, never used again.
     */
    LocalSelectTable();

    static const unsigned char bitsPerBlock = sizeof(BlockType) * 8;
    static const BlockType blockTypeMax = std::numeric_limits<BlockType>::max();

 public:
    static const BlockType npos = std::numeric_limits<BlockType>::max();
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - unsinged char representing a bit vector of size 8
    * @param i-th set bit to be selected
    * @return index of the i-th bit, or (BlockType) - 1 if there is none
    */
    static BlockType getLocalSelect(size_t b, size_t i);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalSelectTable(LocalSelectTable const &) = delete;
    void operator=(LocalSelectTable const &) = delete;
};
template <typename BlockType>
LocalSelectTable<BlockType>::LocalSelectTable() :
    localSelectLookupTable(static_cast<size_t>(blockTypeMax+1)) {
    for (unsigned int i = 0; i <= blockTypeMax; i++) {
        BlockType rank = 0;
        localSelectLookupTable[i] = std::vector<BlockType>(
            bitsPerBlock,
            std::numeric_limits<BlockType>::max());
        for (BlockType j = 0; j < bitsPerBlock; j++) {
            if (((i)>>(j)) & 1) {
                localSelectLookupTable[i][rank++] = j;
            }
        }
    }
}
template <typename BlockType>
BlockType LocalSelectTable<BlockType>::getLocalSelect(size_t b,
                                                      size_t i) {
    static LocalSelectTable<BlockType> instance;
    return instance.localSelectLookupTable[b][i];
}
}  // namespace Sealib
#endif  // SEALIB_LOCALSELECTTABLE_H_

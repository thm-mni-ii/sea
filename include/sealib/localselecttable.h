#ifndef SEALIB_LOCALSELECTTABLE_H_
#define SEALIB_LOCALSELECTTABLE_H_
#include <vector>

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
    ~LocalSelectTable();

    static const unsigned int bitsPerBlock = sizeof(BlockType) * 8;
    static const BlockType blockTypeMax = (BlockType) - 1;


 public:
    static const BlockType npos = blockTypeMax;
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - unsinged char representing a bit vector of size 8
    * @param i-th set bit to be selected
    * @return index of the i-th bit, or (BlockType) - 1 if there is none
    */
    static BlockType getLocalSelect(BlockType segment, BlockType i);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalSelectTable(LocalSelectTable const &) = delete;
    void operator=(LocalSelectTable const &) = delete;
};
}  // namespace Sealib
#endif  // SEALIB_LOCALSELECTTABLE_H_

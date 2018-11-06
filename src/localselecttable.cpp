#include <sealib/localselecttable.h>
#include <sealib/sharedrankstructure.h>
#include <iostream>

template <typename BlockType>
Sealib::LocalSelectTable<BlockType>::LocalSelectTable() : localSelectLookupTable(blockTypeMax+1) {
    for (unsigned int i = 0; i <= blockTypeMax; i++) {
        BlockType rank = 0;
        localSelectLookupTable[i] = std::vector<BlockType>(bitsPerBlock, npos);
        for (BlockType j = 0; j < bitsPerBlock; j++) {
            if (CHECK_BIT(i, j)) {
                localSelectLookupTable[i][rank++] = j;
            }
        }
    }
}
template <typename BlockType>
BlockType Sealib::LocalSelectTable<BlockType>::getLocalSelect(BlockType segment,
                                                              BlockType localIdx) {
    static LocalSelectTable<BlockType> instance;
    return instance.localSelectLookupTable[segment][localIdx];
}
template <typename BlockType>
Sealib::LocalSelectTable<BlockType>::~LocalSelectTable() = default;

namespace Sealib {
template
class LocalSelectTable<unsigned char>;
template
class LocalSelectTable<unsigned short>;
}  // namespace Sealib
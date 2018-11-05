#include <sealib/localranktable.h>
#include <sealib/sharedrankstructure.h>

template <typename BlockType>
Sealib::LocalRankTable<BlockType>::LocalRankTable() : localRankLookupTable(blockTypeMax+1) {
    for (unsigned int i = 0; i <= blockTypeMax; i++) {
        BlockType rank = 0;
        localRankLookupTable[i] = std::vector<BlockType>(bitsPerBlock);
        for (BlockType j = 0; j < bitsPerBlock; j++) {
            if (CHECK_BIT(i, j)) {
                rank++;
            }
            localRankLookupTable[i][j] = rank;
        }
    }
}
template <typename BlockType>
BlockType Sealib::LocalRankTable<BlockType>::getLocalRank(BlockType segment, BlockType localIdx) {
    static LocalRankTable<BlockType> instance;
    return instance.localRankLookupTable[segment][localIdx];
}

template <typename BlockType>
Sealib::LocalRankTable<BlockType>::~LocalRankTable() = default;

namespace Sealib {
template
class LocalRankTable<unsigned char>;
template
class LocalRankTable<unsigned short>;
}  // namespace Sealib
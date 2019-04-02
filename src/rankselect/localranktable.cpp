#include "localranktable.h"
#include <sealib/dictionary/rankstructure.h>

Sealib::LocalRankTable::LocalRankTable() : localRankLookupTable(256) {
    for (uint32_t i = 0; i <= (uint8_t) -1; i++) {
        uint8_t rank = 0;
        localRankLookupTable[i] = std::vector<uint8_t>(8);
        for (uint8_t j = 0; j < 8; j++) {
            if (CHECK_BIT(i, j)) {
                rank++;
            }
            localRankLookupTable[i][j] = rank;
        }
    }
}

uint8_t Sealib::LocalRankTable::getLocalRank(uint8_t segment, uint8_t localIdx) {
    static LocalRankTable instance;
    return instance.localRankLookupTable[segment][localIdx];
}

Sealib::LocalRankTable::~LocalRankTable() = default;

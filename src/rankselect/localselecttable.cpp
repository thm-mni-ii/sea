#include "localselecttable.h"
#include <sealib/dictionary/rankstructure.h>

Sealib::LocalSelectTable::LocalSelectTable() : localSelectLookupTable(256) {
    for (uint32_t i = 0; i <= (uint8_t) -1; i++) {
        uint8_t rank = 0;
        localSelectLookupTable[i] = std::vector<uint8_t>(8, 255);
        for (uint8_t j = 0; j < 8; j++) {
            if (CHECK_BIT(i, j)) {
                localSelectLookupTable[i][rank++] = j;
            }
        }
    }
}

uint8_t Sealib::LocalSelectTable::getLocalSelect(uint8_t segment,
                                                       uint8_t localIdx) {
    static LocalSelectTable instance;
    return instance.localSelectLookupTable[segment][localIdx];
}

Sealib::LocalSelectTable::~LocalSelectTable() = default;

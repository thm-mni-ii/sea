//
// Created by jmeintrup on 24.05.18.
//

#include <include/sealib/localranktable.h>
#include <include/sealib/sharedrankstructure.h>

Sealib::LocalRankTable::LocalRankTable() : localRankLookupTable(256){
    for (unsigned int i = 0; i <=(unsigned char) - 1; i++) {
        unsigned char rank = 0;
        localRankLookupTable[i] = std::vector<unsigned char>(8);
        for (unsigned char j = 0; j < 8; j++) {
            if (CHECK_BIT(i,j)) {
                rank++;
            }
            localRankLookupTable[i][j] = rank;
        }
    }
}

unsigned char Sealib::LocalRankTable::getLocalRank(unsigned char segment, unsigned char localIdx) {
    static LocalRankTable instance;
    return instance.localRankLookupTable[segment][localIdx];
}

Sealib::LocalRankTable::~LocalRankTable() {}

//
// Created by jmeintrup on 24.05.18.
//

#include <include/sealib/localselecttable.h>
#include <include/sealib/rankstructure.h>
#include <iostream>

Sealib::LocalSelectTable::LocalSelectTable() : localSelectLookupTable(256){
    for(unsigned int i = 0; i <= (unsigned char) - 1; i++) {
        unsigned char rank = 0;
        localSelectLookupTable[i] = std::vector<unsigned char>(8,255);
        for (unsigned char j = 0; j < 8; j++) {
            if (CHECK_BIT(i,j)) {
                localSelectLookupTable[i][rank++] = j;
            }
        }
    }
}

unsigned char Sealib::LocalSelectTable::getLocalSelect(unsigned char segment, unsigned char localIdx) {
    static LocalSelectTable instance;
    return instance.localSelectLookupTable[segment][localIdx];
}

Sealib::LocalSelectTable::~LocalSelectTable() {}
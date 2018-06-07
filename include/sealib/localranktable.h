//
// Created by jmeintrup on 24.05.18.
//

#ifndef SEA_LOCALRANKSELECTTABLE_H
#define SEA_LOCALRANKSELECTTABLE_H

#include <vector>

namespace Sealib {
    class LocalRankTable {
    private:
        std::vector<std::vector<unsigned char>> localRankLookupTable;
        LocalRankTable();
        ~LocalRankTable();

    public:
        static unsigned char getLocalRank(unsigned char segment, unsigned char localIdx);

        LocalRankTable(LocalRankTable const&) = delete;
        void operator=(LocalRankTable const&) = delete;
    };
}  // namespace Sealib
#endif //SEA_LOCALRANKSELECTTABLE_H

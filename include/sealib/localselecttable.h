//
// Created by jmeintrup on 24.05.18.
//

#ifndef SEA_LOCALSELECTTABLE_H
#define SEA_LOCALSELECTTABLE_H
#include <vector>

namespace Sealib {
    class LocalSelectTable {
    private:
        std::vector<std::vector<unsigned char>> localSelectLookupTable;
        LocalSelectTable();
        ~LocalSelectTable();

        static LocalSelectTable* instance;

    public:
        static unsigned char getLocalSelect(unsigned char segment, unsigned char localIdx);

        LocalSelectTable(LocalSelectTable const&) = delete;
        void operator=(LocalSelectTable const&) = delete;
    };
}  // namespace Sealib
#endif //SEA_LOCALSELECTTABLE_H

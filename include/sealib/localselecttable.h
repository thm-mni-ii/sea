#ifndef SEALIB_LOCALSELECTTABLE_H_
#define SEALIB_LOCALSELECTTABLE_H_
#include <vector>

namespace Sealib {
/**
* Used for RankSelect to lookup select values in local segments of size 8-bits.
* All values are stored in a lookup table.
* @author Johannes Meintrup
*/
class LocalSelectTable {
 private:
    std::vector<std::vector<unsigned char>> localSelectLookupTable;
    /**
     * Used in getLocalSelect for the first initialization of the table, never used again.
     */
    LocalSelectTable();
    ~LocalSelectTable();

 public:
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - unsinged char representing a bit vector of size 8
    * @param i-th set bit to be selected
    * @return index of the i-th bit, or (unsinged char) - 1 if there is none
    */
    static unsigned char getLocalSelect(unsigned char segment, unsigned char i);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalSelectTable(LocalSelectTable const &) = delete;
    void operator=(LocalSelectTable const &) = delete;
};
}  // namespace Sealib
#endif  // SEALIB_LOCALSELECTTABLE_H_

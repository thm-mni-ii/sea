#ifndef SRC_RANKSELECT_LOCALSELECTTABLE_H_
#define SRC_RANKSELECT_LOCALSELECTTABLE_H_
#include <cstdint>
#include <vector>

namespace Sealib {
/**
* Used for RankSelect to lookup select values in local segments of size 8-bits.
* All values are stored in a lookup table.
* @author Johannes Meintrup
*/
class LocalSelectTable {
 private:
    std::vector<std::vector<uint8_t>> localSelectLookupTable;
    /**
     * Used in getLocalSelect for the first initialization of the table, never used again.
     */
    LocalSelectTable();
    ~LocalSelectTable();

 public:
    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment unsigned char representing a bit vector of size 8
    * @param i set bit to be selected
    * @return index of the i-th bit, or (unsinged char) - 1 if there is none
    */
    static uint8_t getLocalSelect(uint8_t segment, uint8_t i);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalSelectTable(LocalSelectTable const &) = delete;
    void operator=(LocalSelectTable const &) = delete;
};
}  // namespace Sealib
#endif  // SRC_RANKSELECT_LOCALSELECTTABLE_H_

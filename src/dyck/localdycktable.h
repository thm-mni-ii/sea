#ifndef SRC_DYCK_LOCALDYCKTABLE_H_
#define SRC_DYCK_LOCALDYCKTABLE_H_

#include <cstdint>
#include <vector>

namespace Sealib {
/**
 * Lookup Table for local dyck matches, depths, and base pioneers
 * @author Johannes Meintrup
 */
class LocalDyckTable {
 public:
    static constexpr const uint32_t kSegLen = 7;
    /**
     * Capsules the Data we want to lookup in the table in a single class.
     */
    class Data {
     public:
        uint8_t localMatches[kSegLen];  // array of local matches
        char localDepths[kSegLen];  // array of local depths
        uint8_t leftPioneer;  // left pioneer, if there is one
        uint8_t rightPioneer;  // right pioneer, if there is one

        /**
         * @param segment to be used for data creation, meaning the local dyck segment
         */
        explicit Data(uint8_t segment);
        Data();
    };

    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - uint8_t representing a bit vector of size 8
    * @return Local Dyck Data matching the segment
    */
    static const Data& getLocalData(uint8_t segment);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalDyckTable(LocalDyckTable const &) = delete;
    void operator=(LocalDyckTable const &) = delete;

 private:
    std::vector<Data> table;

    LocalDyckTable();
    ~LocalDyckTable() = default;
};
}  // namespace Sealib
#endif  // SRC_DYCK_LOCALDYCKTABLE_H_

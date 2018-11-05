#ifndef SEALIB_LOCALDYCKTABLE_H_
#define SEALIB_LOCALDYCKTABLE_H_

#include <vector>

namespace Sealib {

template <typename BlockType = unsigned char>
class LocalDyckTable;
}

namespace Sealib {
/**
 * Lookup Table for local dyck matches, depths, and base pioneers
 * @author Johannes Meintrup
 */
template <typename BlockType>
class LocalDyckTable {
 public:

    static const constexpr unsigned int bitsPerBlock = sizeof(BlockType) * 8;
    static const constexpr BlockType blockTypeMax = (BlockType) - 1;

    static const constexpr unsigned int kSegLen = bitsPerBlock - 1;
    /**
     * Capsules the Data we want to lookup in the table in a single class.
     */
    class Data {
     public:
        unsigned char localMatches[kSegLen];  // array of local matches
        char localDepths[kSegLen];  // array of local depths
        unsigned char leftPioneer;  // left pioneer, if there is one
        unsigned char rightPioneer;  // right pioneer, if there is one

        /**
         * @param segment to be used for data creation, meaning the local dyck segment
         */
        explicit Data(BlockType segment);
        Data();
        ~Data() = default;
    };

    /**
    * Values are stored in a lookup table.
    * The lookup table is a static instance in this function that is initialized on the first call.
    * @param segment - unsigned char representing a bit vector of size 8
    * @return Local Dyck Data matching the segment
    */
    static const Data& getLocalData(BlockType segment);

    //  singleton instance only in the function getLocalSelect, these should not be used
    LocalDyckTable(LocalDyckTable const &) = delete;
    void operator=(LocalDyckTable const &) = delete;

 private:
    std::vector<Data> table;

    LocalDyckTable();
    ~LocalDyckTable();
};
}  // namespace Sealib
#endif  // SEALIB_LOCALDYCKTABLE_H_

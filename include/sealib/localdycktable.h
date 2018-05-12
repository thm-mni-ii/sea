//
// Created by jmeintrup on 02.05.18.
//

#ifndef SEA_LOCALDYCKTABLE_H
#define SEA_LOCALDYCKTABLE_H

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

namespace Sealib {
/**
 * Lookup Table for local dyck matches, depths, and base pioneers
 * @author Johannes Meintrup
 */
class LocalDyckTable {
 public:
    /**
     * Capsules the Data we want to lookup in the table in a single class.
     */
    class Data {
     public:
        unsigned char* localMatches;  // array of local matches
        char* localDepths;  // array of local depths
        unsigned char leftPioneer;  // left pioneer, if there is one
        unsigned char rightPioneer;  // right pioneer, if there is one

        /**
         * @param len length of the segment i.e. length of the arrays
         */
        explicit Data(unsigned char len);

        /**
         * destructor
         */
        virtual ~Data();
    };
    /**
     * @return number of entries in the table
     */
    unsigned long getEntries() const;
    /**
     * creates a new Data object for a given segment g, with length len
     * @param g segment to be interpreted as a dyckword, set bits are "(", unset bits ")".
     * @param len length of the segment
     * @return Data
     */
    static Data* calculateLocalData(unsigned long g, unsigned char len);

    /**
     * @param i index
     * @return table[i]
     */
    const Data* operator[](unsigned long i);

    /**
     * @param len length of the segments
     */
    explicit LocalDyckTable(unsigned char len);

    /**
     * default
     */
    LocalDyckTable();

    /**
     * default destructor
     */
    virtual ~LocalDyckTable();

 private:
    Data **table;
    unsigned long entries;
};
}  // namespace Sealib
#endif  // SEA_LOCALDYCKTABLE_H

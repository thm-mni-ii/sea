#ifndef SRC_DYCK_NAIVEDYCKMATCHINGSTRUCTURE_H_
#define SRC_DYCK_NAIVEDYCKMATCHINGSTRUCTURE_H_

#include "dyckmatchingstructure.h"

namespace Sealib {
/**
 * Space efficient implementation of a dyck matching structure.
 * Enables to find the match of a given parenthesis b in a dyckword w in
 * constant time.
 */
class NaiveDyckMatchingStructure : public DyckMatchingStructure {
 public:
    /**
     * Constructs the matching structure for the given dyck word word_
     * The structure is built for the dyck word d_p
     * @param word_ - valid dyck word. validity is not being tested
     */
    explicit NaiveDyckMatchingStructure(const Sealib::Bitset<uint8_t> &word_);

    /**
     * finds the match of a parenthesis b in word in constant time
     * @param idx of the parenthesis b in word
     * @return idx of the match of b in word
     */
    uint64_t getMatch(uint64_t idx) override;

 private:
    std::vector<uint32_t> table;
    std::vector<uint32_t> initializeTable(const Sealib::Bitset<uint8_t> &word_);
};
}  // namespace Sealib
#endif  // SRC_DYCK_NAIVEDYCKMATCHINGSTRUCTURE_H_

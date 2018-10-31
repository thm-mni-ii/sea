#ifndef SEALIB_STATICSPACESTORAGE_H_
#define SEALIB_STATICSPACESTORAGE_H_
#include <cstdint>
#include <vector>
#include "sealib/_types.h"
#include "sealib/bitset.h"
#include "sealib/rankselect.h"

namespace Sealib {
class StaticSpaceStorage {
 public:
    uint get(uint i) const;

    void insert(uint i, uint v);

    explicit StaticSpaceStorage(const std::vector<bool> &bits);

    static std::vector<bool> makeBitVector(std::vector<uint> *sizes);

 private:
    Bitset<uint_fast8_t> pattern;
    const RankSelect rankSelect;
    Bitset<uint_fast8_t> storage;
};
}  // namespace Sealib
#endif  // SEALIB_STATICSPACESTORAGE_H_

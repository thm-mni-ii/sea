#ifndef SEALIB_STATICSPACESTORAGE_H_
#define SEALIB_STATICSPACESTORAGE_H_
#include <cstdint>
#include <vector>
#include "sealib/_types.h"
#include "sealib/bitset.h"

using Sealib::Bitset;

namespace Sealib {
class StaticSpaceStorage {
 public:
  explicit StaticSpaceStorage(const std::vector<bool>& pattern);
  ~StaticSpaceStorage();

  uint get(uint i);

  void set(uint i, uint v);

 private:
  typedef uint_fast8_t Unit;
  typedef Unit *Alloc;
  Alloc storage;
  static Unit bitsToUnit(const std::vector<bool>&);
};
}  // namespace Sealib
#endif  // SEALIB_STATICSPACESTORAGE_H_

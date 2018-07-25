#ifndef SEALIB_STATICSPACESTORAGE_H_
#define SEALIB_STATICSPACESTORAGE_H_
#include <vector>
#include "sealib/_types.h"
#include "sealib/bitset.h"

using Sealib::Bitset;

#define Byte unsigned char
typedef Bitset<Byte> Alloc;

namespace Sealib {
class StaticSpaceStorage {
 public:
  explicit StaticSpaceStorage(const std::vector<bool>& pattern);
  ~StaticSpaceStorage();

  uint get(uint i);

  void set(uint i, uint v);

 private:
  Alloc **storage;
};
}  // namespace Sealib
#endif  // SEALIB_STATICSPACESTORAGE_H_

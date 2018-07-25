#include "sealib/staticspacestorage.h"

using Sealib::StaticSpaceStorage;

uint StaticSpaceStorage::get(uint i) {
    Bitset<Byte> b=*storage[i];
    uint r=0;
    printf("bytesize=%lu\n",sizeof(Byte));
    for(uint a=0; sizeof(uint)>=sizeof(Byte)&&b.blocks()>a; a++) {
        r|=(b.getBlock(a)<<a*sizeof(Byte));
        printf("block %u=%u\n",a,b.getBlock(a));
    }
    return r;
}

void StaticSpaceStorage::set(uint i,uint v) {
    Bitset<Byte>& b=*storage[i];
    for(uint a=0; sizeof(uint)>=sizeof(Byte)&&b.blocks()>a; a++) {
        Byte c=(v>>a*sizeof(Byte))&0xFF;
        b.setBlock(a,c);
        printf("block %u<-%u\n",a,b.getBlock(a));
    }
}

StaticSpaceStorage::StaticSpaceStorage(const std::vector<bool>& pattern) {
  uint size = 0;
  for (bool a : pattern)
    if (a == 1) size++;
  bool first = true;
  storage = new Alloc*[size];
  uint i, n;
  for (bool a : pattern) {
    printf("a=%u, i=%u, n=%u\n", a, i, n);
    if (a == 1) {
      if (first) {
        first = false;
        i = 0;
        n = 1;
      } else {
        storage[i] = new Alloc(n);
        i++;
        n = 1;
      }
    } else {
      n++;
    }
  }
  storage[i] = new Alloc(n);
}

StaticSpaceStorage::~StaticSpaceStorage() { delete[] storage; }

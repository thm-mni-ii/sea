#include "sealib/staticspacestorage.h"

using Sealib::StaticSpaceStorage;

/*uint StaticSpaceStorage::get(uint i) {
    Bitset<Byte> b=storage[i];
    uint r=0;
    printf("bytesize=%u\n",sizeof(Byte));
    for(uint a=0; sizeof(uint)>=sizeof(Byte)&&b.blocks()>a; a++) {
        r|=(b.getBlock(a)<<a*sizeof(Byte));
        printf("block %u=%u\n",a,b.getBlock(a));
    }
    return r;
}

void StaticSpaceStorage::set(uint i,uint v) {
    Bitset<Byte>& b=storage[i];
    for(uint a=0; sizeof(uint)>=sizeof(Byte)&&b.blocks()>a; a++) {
        Byte c=(v>>a*sizeof(Byte))&0xFF;
        b.setBlock(a,c);
        printf("block %u<-%u\n",a,b.getBlock(a));
    }
}*/

Sealib::StaticSpaceStorage::Unit StaticSpaceStorage::bitsToUnit(const std::vector<bool>& p) {
  Unit n=0;
  Unit b=0;
  for(bool a:p) {
    n|=static_cast<Unit>(a<<(8*sizeof(Unit)-1-b));
    printf("n=%u ",n);
    b++;
  }
  return n;
}
StaticSpaceStorage::StaticSpaceStorage(const std::vector<bool>& pattern) {
  uint size = pattern.size()/(8*sizeof(Unit))+1;
  storage = new Unit[size];
  uint i;
  std::vector<bool> b=std::vector<bool>();
  for(uint a=0; a<size; a++) {
    for(uint c=0; c<8*sizeof(Unit); c++) {
      i=a*8*sizeof(Unit)+c;
      if(i<pattern.size()) {
        printf("%u \t b :: %u\n",i,pattern[i]);
        b.push_back(pattern[i]);
      } else {
        printf("%u \t b :: 0\n",i);
        b.push_back(0);
      }
    }
    storage[a]=bitsToUnit(b);
    b.clear();
    printf("storage[%u]=0x%x \t Unit=%u\n", a,storage[a],sizeof(Unit));
  }
}

StaticSpaceStorage::~StaticSpaceStorage() { delete[] storage; }

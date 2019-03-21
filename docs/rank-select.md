Rank-Select
===
A rank-select data structure is a data structure initialized for some bitset B={b_0,...,b_n} and supports the operations *rank(i)* and *select(i)* with 0 < i <= n.

## Operations
* rank(i): Returns the number of bits set to 1 in the bitset B up to and including to the index i-1, if i=0 or i>n, the operation returns (uint64)-1.
* select(i): Returns the index j+1 of the i'th bit set to 1. If the Bitset has less than i bits set to 1, it returns (uint64)-1 (or if i=0 or i>n).

## Efficiency
* Time: O(n) Initialization, O(1) *rank* and *select*
* Space: O(n)

## Example
```cpp
#include <sealib/dictionary/rankselect.h>

void main() {
	Sealib::Bitset<uint8_t> b(10);
	b[1] = 1;
	b[4] = 1;
	b[7] = 1;
	//b = [0,1,0,0,1,0,0,1,0,0]

	Sealib::RankSelect<uint8_t> rs(b);
	rs.rank(1); // = 0
	rs.rank(2); // = 1
	rs.rank(3); // = 1
	rs.rank(0); // = (uint64_t) - 1 - illegal argument
	rs.rank(11); // = (uint64_t) - 1

	rs.select(1); // = 2
	rs.select(2); // = 5
	rs.select(3); // = 8
	rs.select(4); // = (uint64_t)-1 - only 3 bits set to 1 in b
	rs.select(0); // = (uint64_t)-1 - illegal argument;
}
```



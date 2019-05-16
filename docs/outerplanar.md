Outerplanarity Detection
===
This algorithm detects whether a biconnected input graph is *biconnected outerplanar* or not.
- The algorithm **requires a biconnected input graph**: to create one, run the [BCC output](./bcc-iterator.md) on an arbitrary graph and feed the resulting BCCs to this algorithm one by one.

This space-efficient outerplanar detection
- uses a virtual graph consisting of a bit vector and a [ragged dictionary](./ragged-dictionary.md)
- uses a compact array with [rank-select](./rank-select.md) for path counters

## Efficiency
- Time: O(n log(log(n)))
- Space: O(n) bits

## Example
```cpp
#include "sealib/iterator/outerplanarchecker.h"
#include "sealib/graph/graphcreator.h"

int main() {
    Sealib::UndirectedGraph g = Sealib::GraphCreator::cycle(5000, 8);

    Sealib::OuterplanarChecker c(g);
    if(c.isOuterplanar()) {
        // do something with the outerplanar graph
    }
}
```
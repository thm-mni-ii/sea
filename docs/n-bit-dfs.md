O(n)-Bit Depth-First Search
===
The depth-first search over a graph G=(V,E) will *process* every node and *explore* every edge exactly once. The user can supply four *user-defined procedures*: `preprocess`, `preexplore`, `postexplore` and `postprocess`.

This space-efficient variant
- uses a compact bitset to store the *node colors*
- uses a segment stack which only keeps the two top-most segments and the last *trailer*.
    - When both segments are full and a push occurs, the lower segment is discarded.
    - When both segments are empty and a pop occurs, a restoration is started. The restoration is simply a *quiet* run (user procedures are disabled) of the DFS from the beginning. It runs until the current trailer matches with the trailer before the restoration.

## Efficiency
* Time: O((n+m) log n)
* Space: O((log(3)+ε) n) bits

## Example
```cpp
#include <cstdio>
#include "sealib/iterator/dfs.h"
#include "sealib/graph/graphcreator.h"
using Sealib::DFS;

// example procedures:
void preproc(uint64_t u) { printf("preprocess %u\n", u); }
void postproc(uint64_t u) { printf("postprocess %u\n", u); }
void preexp(uint64_t u, uint64_t v) { printf("preexplore %u,%u\n", u, v); }
void postexp(uint64_t u, uint64_t v) { printf("postexplore %u,%u\n", u, v); }

int main() {
    DirectedGraph g = Sealib::GraphCreator::sparseDirected(50);
    DFS::nBitDFS(g);  // quiet run

    DFS::nBitDFS(g, preproc, preexp, postexp, postproc);  // supply procedures to do something with the current node or edge
}
```
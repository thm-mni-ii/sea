O(n+m)-Bit Depth-First Search
===
The depth-first search over a graph G=(V,E) will *process* every node and *explore* every edge exactly once. The user can supply four *user-defined procedures*: `preprocess`, `preexplore`, `postexplore` and `postprocess`.

This space-efficient variant *only works with undirected graphs*:
- A static-space allocation keeps the backwards edge index for each node.
    - By looking up the index, we know which node is the parent node. 
    - Another trick helps to find the next edge of the parent: Since each adjacency in an undirected graph also holds *cross indices*, the next edge that the parent will explore is `mate(childNode,backwardsEdge)+1`.
- A compact bitset is used to store the *node colors*.

## Efficiency
* Time: O(n+m)
* Space: O(n+m) bits

## Example
```cpp
#include <cstdio>
#include "sealib/iterator/dfs.h"
#include "sealib/graph/graphcreator.h"
using Sealib::DFS;

// example procedures:
void preproc(uint64_t u) { printf("preprocess %u\n", u); }
void postproc(uint64_t u) { printf("postprocess %u\n", u); }
// ! Attention: for this DFS variant, the 'explore' calls return the edge INDEX k, not the node v
void preexp(uint64_t u, uint64_t k) { printf("preexplore %u,%u\n", u, k); }
void postexp(uint64_t u, uint64_t k) { printf("postexplore %u,%u\n", u, k); }

int main() {
    Sealib::UndirectedGraph g = Sealib::GraphCreator::kRegular(100,5);
    DFS::nBitDFS(g);  // quiet run

    DFS::nBitDFS(g, preproc, preexp, postexp, postproc);  // supply procedures to do something with the current node or edge
}
```
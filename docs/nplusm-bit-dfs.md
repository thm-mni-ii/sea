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
UndirectedGraph g=GraphCreator::kRegular(100,5)

DFS::nplusmBitDFS(&g);  // quiet run

DFS::nplusmBitDFS(&g, p0, e0, e1, p1);  // supply procedures to do something with the current node or edge


// example procedures:
void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }
```
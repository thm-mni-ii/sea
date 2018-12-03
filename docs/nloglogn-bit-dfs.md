O(n*log(log(n)))-Bit Depth-First Search
===
The depth-first search over a graph G=(V,E) will *process* every node and *explore* every edge exactly once. The user can supply four *user-defined procedures*: `preprocess`, `preexplore`, `postexplore` and `postprocess`.

This space-efficient variant
- uses a compact bitset to store the *node colors*
- uses a segment stack which only keeps the two top-most segments and the last *trailer*.
    - When both segments are full and a push occurs, the lower segment is discarded.
    - When both segments are empty and a pop occurs, a restoration is started. The restoration will recreate exactly one segment by starting at the second-last trailer and continuously pushing the next gray node located in the top segment.
- speeds up the restoration by not searching through all outgoing edges:
	- A compact bitset stores edge approximations for *small vertices* in O(n log(log(n))) bits total
	- The outgoing edges of *big vertices* are stored separately (there are at most n/log(n) big vertices so that they occupy no more than O(n) bits)

## Efficiency
* Time: O(n+m)
* Space: O(n log(log(n))) bits

## Example
```cpp
DirectedGraph g=GraphCreator::createRandomImbalanced(500);

DFS::nloglognBitDFS(&g);  // quiet run

DFS::nBitDFS(&g, p0, e0, e1, p1);  // supply procedures to do something with the current node or edge


// example procedures:
void p0(uint u) { printf("preprocess %u\n", u); }
void p1(uint u) { printf("postprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
void e1(uint u, uint v) { printf("postexplore %u,%u\n", u, v); }
```

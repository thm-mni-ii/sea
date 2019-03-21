O(n)-Bit Breadth-First Search
===
The breadth-first search over a graph G=(V,E) will find all *connected components* of G and output the tuple (u,dist) for each node, which is shows the distance of u to the starting node of the current component. Two *user-defined procedures* are available: `preprocess` and `preexplore`.

This space-efficient variant
- uses a compact bitset to store the *node colors*
- is built as an iterator which *streams* the results to the user:
	- *init()*: initializes the BFS
	- *more()*: returns true if there is more to do in this connected component
	- *next()*: gets the next node and distance from the component
	- *nextComponent()*: advances to the next component, or returns false if the graph is completely explored

## Efficiency
* Time: O(n+m)
* Space: O(n) bits

## Example
```cpp
#include <cstdio>
#include "sealib/iterator/bfs.h"
#include "sealib/graph/graphcreator.h"

// example procedures:
void preproc(uint64_t u) { printf("preprocess %u\n", u); }
void preexp(uint64_t u, uint64_t v) { printf("preexplore %u,%u\n", u, v); }

int main() {
    Sealib::DirectedGraph g = Sealib::GraphCreator::kOutdegree(500,2);

    Sealib::BFS bfs(g, preproc, preexp);
    bfs.init();		// don't forget initialization of the iterator
    bfs.forEach([&](std::pair<uint64_t, uint64_t>) { 
        uint64_t u=s.first, dist=s.second;
        std::cout << "found vertex " << u << " with distance " << dist << "\n";
    });
}
```

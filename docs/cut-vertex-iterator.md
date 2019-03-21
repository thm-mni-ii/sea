Cut-Vertex Iterator
===
A *cut vertex* is a vertex of an undirected, connected graph G=(V,E) that will *disconnect* the graph if it is removed. To find the cut vertices in a graph, the edges are classified into tree, back and cross/forward edges using a DFS. Another DFS finds the *edge markings* for all the tree edges of G.

This space-efficient variant
- uses the O(n+m)-Bit DFS to identify and mark edges
- uses a static-space allocation to hold the edge markings
- is built as an iterator:
    - *init()*: initializes the iterator (identifies and marks tree edges)
    - *more()*: returns true if there are more cut vertices in the graph
    - *next()*: gets the next found cut vertex
    - **isCutVertex(u)**: Special function that can be called at any time after init(). Returns true if **u** is a cut vertex, false otherwise.

## Efficiency
* Time: O(n+m)
* Space: O(n+m) bits

## Example
```cpp
#include <cstdio>
#include "sealib/iterator/cutvertexiterator.h"
#include "sealib/graph/graphcreator.h"

int main() {
    Sealib::UndirectedGraph g = Sealib::GraphCreator::windmill(3,4);

    Sealib::CutVertexIterator c(g);
    c.init();
    while(c.more()) {
        std::cout << "found: " << c.next() << "\n";
    }

    printf("center should be cut vertex: %d\n",c.isCutVertex(g.getOrder()-1));
}
```
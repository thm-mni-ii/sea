Biconnected-Component Output
===
A *biconnected component* (BCC) is a subgraph of G=(V,E) that is biconnected: at least 2 vertices must be removed from it to *disconnect* the component. That means that a biconnected graph can never contain a [cut vertex](cut-vertex-iterator.md).

To find the BCCs in a graph, the edges are classified into tree, back and cross/forward edges using a DFS. Another DFS finds the *edge markings* for all the tree edges of G. It is then possible to supply a start vertex *u0* and get all the nodes and edges that belong to the same BCC as *u0*.

This space-efficient variant
- uses the O(n+m)-Bit DFS to identify and mark edges
- uses a static-space allocation to hold the edge markings
- has the following method:
    - *traverse(u0, onVertex, onEdge)*: traverses the entire BCC of *u0*, calls *onVertex(u)* for each vertex *u* belonging to the BCC and *onEdge(u,v)* for each edge *{u,v}* belonging to the BCC

## Efficiency
* Time: O(n+m)
* Space: O(n+m) bits

## Example
```cpp
#include <cstdio>
#include "sealib/iterator/bcciterator.h"
#include "sealib/graph/graphcreator.h"

int main() {
    Sealib::UndirectedGraph g = Sealib::GraphCreator::windmill(3,4);

    Sealib::BCCOutput b(g);
    b.traverse(2,
        [](uint64_t u){
            printf("found vertex: %lu\n", u); 
        },
        [](uint64_t u, uint64_t v) {
            printf("found edge: %lu,%lu\n", u, v);
        });
}
```

Biconnected-Component Iterator
===
An iterative version of the above algorithm.
- *init(u,v)*: iterate over the BCC belonging to edge *{u,v}*
- *more()*: returns true if there are more vertices/edges in the BCC
- *next()* returns a pair holding the next element: either *(u,INVALID)* for vertex *u*, or *(u,v)* for edge *{u,v}*

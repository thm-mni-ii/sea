Biconnected-Component Iterator
===
A *biconnected component* (BCC) is a subgraph of G=(V,E) that is biconnected: at least 2 vertices must be removed from it to *disconnect* the component. That means that a biconnected graph can never contain a [cut vertex](cut-vertex-iterator.md).

To find the BCCs in a graph, the edges are classified into tree, back and cross/forward edges using a DFS. Another DFS dinds the *edge markings* for all the tree edges of G. It is then possible to supply an edge (u,v) and get all the nodes and edges that belong to the same BCC as the edge.

This space-efficient variant
- uses the O(n+m)-Bit DFS to identify and mark edges
- uses a static-space allocation to hold the edge markings
- is built as an iterator:
    - *init()*: initializes the iterator (identifies and marks tree edges)
    - *start(u,v)*: sets the starting edge (u,v)
    - *more()*: returns true if there are more nodes/edges in the current BCC
    - *next()*: gets the next node/edge found in the BCC
        - if a node is found, the returned tuple is (u,INVALID)
        - if an edge is found, the returned tuple is (u,v)

## Efficiency
* Time: O(n+m)
* Space: O(n+m) bits

## Example
```cpp
UndirectedGraph g=GraphCreator::windmill(3,4);

BCCIterator b(&g);
b.init();
b.start(1,2);
b.forEach([](std::pair<uint,uint> p) {
    if(p.second==INVALID) {
        std::cout << "found node: " << p.first << "\n";
    } else {
        std::cout << "found edge: " << p.first << "," << p.second << "\n";
    }
});
```
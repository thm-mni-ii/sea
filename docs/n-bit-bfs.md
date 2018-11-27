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
DirectedGraph g=GraphCreator::createRandomFixed(500,2);

BFS bfs(&g, p0, e0);
bfs.init();		// don't forget initialization of the iterator
do {
	while(bfs.more()) {
		std::pair&lt;uint,uint&gt; s=bfs.next();
		uint u=s.first, dist=s.second;
		// ...
	}
} while(bfs.nextComponent());


// example procedures:
void p0(uint u) { printf("preprocess %u\n", u); }
void e0(uint u, uint v) { printf("preexplore %u,%u\n", u, v); }
```

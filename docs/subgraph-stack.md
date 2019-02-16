Subgraph-Stack
===
Stack structure used to manage subgraph storage and access for recursive graph algorithms. Manages a stack of undirected graphs (the member list) {G_0, ..., G_l}  such that G_i is a subgraph of G_{i-1} for each 0 < i < l+i. Each graph on the stack can be accessed and implements the graph access interface (*head(v,k)*, *mate(v,k)*, *deg(v)*, *order()*). The graphs on the stack are not the actual subgraph, but isomorph to the subgraph. I.e. from the view of the user each subgraph G_i(V_i, E_i) has sets V_i={1,...,n_i} and the equivalent set E_i. The structure provides various translation operations to translate from a graph G_i to some other graph G_j on the stack. 

## Operations
- *push(B_V,B_A)* pushes a new subgraph G_{l+1} on the stack. B_V is a bitset of length n_l with bits set to *1* at an index v if G_{l+1} contains the vertex v \in V_l. B_A is a bitset of length 2m_l with bits set to *1* at an index a and a' if G_{l+1} contains the edge represented by the arcs a and a' of G_l.

- *push(B_A)* pushes a new subgraph with the bitset B_V induced by B_A, i.e. the subgraph contains all vertices connected by the arcs in B_A.

- *pop* removes the top subgraph on the stack.

- *order(i=l)* returns the order of G_i

- *degree(i=l, u)* returns the degree of vertex u in G_i

- *head(i=l, u, k)* returns the head of u's k-th outgoing arc in G_i

- *mate(i=l, u, k)* returns the pair that represents the mate of u's k-th outgoing arc in G_i

- *gMax(i=l)* returns the number of arcs the subgraph G_i has

- *g(i=l, u, k)* returns the arc number of the k'th outgoing arc in vertex u in G_i. The arc number is defined as:
      <img src="https://latex.codecogs.com/svg.latex?g(u,k)=k+\sum_{i=1}^{u-1}d_i" title="g(u,k)=k+\sum_{i=1}^{u-1}d_i" /> 

- *gInv(i=l, r)* returns the (vertex, arc) pair belonging to the r'th arc in G_i. I.e. the inverse of the arc numbering function

- *phi(i=l, j=0, u)* translation of the u'th vertex in in G_i to the isomorpth vertex in G_j

- *psi(i=l, j=0, a)* translation of the a'th arc in G_i to the isomorph arc in G_j

- *size()* returns l=the size of the stack

- *SubGraphStack(G)* initialized a new subgraph-stack for the undirectd graph G

## Efficiency
- All access and translation functions take O(1) time
- Pushing a new subgraph takes O(n+m) time.
- Space: The entire subgraphstack occupies O(n+m) bits space.

## Example
Creates a new subgraph stack for some randomly generated undirected graph bg.
Pushes 10 new subgraphs on the stack, with each new subgraph containing 3/4 of the arcs of its parent graph. With the size of each subgraph shrinking according to this rule the size of the entire subgraph will be O(n+m) bits.

```cpp
#include <sealib/collection/subgrpahstack.h>
#include <sealib/graph/graphcreator.h>

void main() {
	std::shared_ptr<Sealib::Graph> bg = ;
		Sealib::GraphCreator::generateRandomBipartiteBasicGraph(1000, 1000, 0.1, 1);

	Sealib::SubgraphStack stack(bg);
	
	for(uint64_t i = 0; i < 10; i++) {
		Sealib::Bitset<uint8_t> a(stack.gMax(u), (uint_8) - 1);
		
		for (uint64_t j = 0; j < a.size(); j++) {
			if (j % 4 == 0) {
			std::tuple<uint64_t, uint64_t> gInv = stack.gInv(j + 1);
			std::tuple<uint64_t, uint64_t> mate 
			    = stack.mate(std::get<0>(gInv), std::get<1>(gInv));
			    uint64_t mateArc = stack.g(std::get<0>(mate), std::get<1>(mate));
			a[j] = 0;
			a[mateArc - 1] = 0;
        	    }
        	}
 	       stack.push(a);	
	}
}
```


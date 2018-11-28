Inplace - Breadth-First Search
===
The breadth-first search over a graph G=(V,E) will find all *connected components* of G. Two *user-defined procedures* are available: `preprocess(uint node, uint round_number)` and `preexplore(uint node, uint parentnode, uint round_number)`. Before the iteration over the Graph can start, we need to verify the Graph is in shifted representation. This is needed in order to save a choice dictionary inside the node array.

### This space-efficient variant
- (currently) Uses a compactarray to store the *node colors*
- works in the restore model
- while the algorithm is processing, the graphstructures integrity is not given


## Efficiency
* Time: O(n+m)
* Space: O(1) bits

## Example
```cpp
std::mt19937_64 gen;
gen.seed(time(0));
uint n = 1000;
double p = 0.5;

CompactGraph g = GraphCreator::generateGilbertGraph(n,p,&gen);
Graphrepresentations::standardToShifted(g.getData());
ChoiceDictionaryDummy color(n)
InplaceBFS inplacebfs(&g,&color,p0,e0);
inplacebfs.runInplaceBFS();
Graphrepresentations::shiftedToStandard(g.getData());

// example procedures:
void p0(uint ui, uint round_number) { printf("preprocess %u\n", u); }
void e0(uint u, uint v, uint round_number) { printf("preexplore %u,%u\n", u, v); }
```

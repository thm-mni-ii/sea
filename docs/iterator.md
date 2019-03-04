Iterator
===
*Iterator* is an interface that defines the piecewise traversal over a result set. When using an iterator, we need no additional space to hold the results of an algorithm: instead, the results are *streamed* to the user one by one.

Any class that implements this interface has the following methods:
- *init()*: initializes the iterator
- *more()*: returns true if there are more undiscovered elements
- *next()*: gets the next element
- *forEach(F)*: execute the given function F for each element

## Implementing classes
- BCCIterator
- BFS
- ChoiceDictionaryIterator
- CutVertexIterator
- ReverseDFS
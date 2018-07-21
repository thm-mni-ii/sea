Choice Dictionary
===
A choice dictionary is a bitset containing n elements that supports reading and setting a bit in constant time and additionally a so-called *choice* operation that returns the position of an arbitrary bit that is set to 1 in constant time.

## Operations
* get(i): Returns the bit at index i.
* insert(i): Set a the bit an index i to 1.
* choice: Returns an arbitrary bit position that is set to 1.

## Efficiency
* Time: O(1) (all operations)
* Space: O(n log n)

## Example

```cpp
ChoiceDictionary* cd = new ChoiceDictionary(12);
cd.insert(0); // Indexing beginns with 0
cd.insert(4);
cd.insert(7);
cd.isnert(11);

cd.get(0); // Returns 1
cd.get(2); // Returns 0

cd.choice(); // May return 0, 4, 7 or 11.

delete cd;
```


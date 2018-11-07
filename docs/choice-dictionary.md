Choice Dictionary
===
A choice dictionary is a bitset containing n elements that supports reading and setting a bit in constant time and additionally a so-called *choice* operation that returns the position of an arbitrary bit that is set to 1 in constant time.

## Operations
* get(i): Returns the bit at index i.
* insert(i): Set the bit at index i to 1.
* remove(i): Set the bit at index i to 0.
* choice: Returns an arbitrary bit position that is set to 1.

## Efficiency
* Time: O(1) (all operations)
* Space: O(n) bits

## Example

```cpp
ChoiceDictionary* cd = new ChoiceDictionary(12);
cd.insert(0); // Indexing beginns with 0
cd.insert(4);
cd.insert(7);
cd.insert(11);

cd.get(0); // Returns 1
cd.get(2); // Returns 0

cd.choice(); // May return 0, 4, 7 or 11.

delete cd;
```
<br>

ChoiceDictionaryIterator
===

A ChoiceDictionaryIterator is used to iterate through all bits set to 1 in a choice dictionary.
It supports the so-called *more* operation that returns true if the choice dictionary contains more bits set to 1, aswell as the *next* operation that returns the index of the next arbitrary bit.

## Operations
* init: Initializes the ChoiceDictionaryIterator.
* more: Returns true if there are more bits set to 1.
* next: Returns the next arbitrary bit position that is set to 1.

## Efficiency
* Time: O(1) (all operations)

## Example

```cpp
ChoiceDictionary* cd = new ChoiceDictionary(12);
ChoiceDictionaryIterator* it = new ChoiceDictionaryIterator(cd);
cd->insert(0); // Indexing beginns with 0
cd->insert(4);
cd->insert(7);
cd->insert(11);

it->init();

while (it->more()) { // Returns true if more bits are set to 1
    it->next(); // May return the next arbitrary bit 0, 4, 7 or 11.
}

delete it;
delete cd;
```

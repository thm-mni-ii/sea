Ragged Dictionary
===

A ragged dictionary is a data structure that manages a subset of the universe *{0,...,n-1}*. The dictionary contains at most n/log(n) key-value tuples.

## Operations
* member(k): Checks if *k* is a member of the dictionary.
* someId(): Returns an arbitrary key managed by the dictionary, or INVALID if the dictionary is empty.
* allIds(): Returns a [*ChoiceDictionaryIterator*](./choice-dictionary.md/#choicedictionaryiterator) over all keys in the dictionary.
* get(k): Returns the value stored for key *k*.
* insert(k,v): Inserts or updates the key *k* with the value *v*.
* remove(k): Removes the tuple with key *k* from the dictionary.

## Efficiency
* Time: O(1) (member, someId, allIds) ~ O(log(log(n))) (get, insert, remove)
* Space: O(n) bits

## Example

```cpp
#include "sealib/dictionary/raggeddictionary.h"

int main() {
    Sealib::RaggedDictionary d(1000);
    d.insert(0,17);
    d.insert(4,5);
    d.insert(5,4);
    // ...
    d.remove(d.someId());
    Sealib::ChoiceDictionaryIterator ci = d.allIds();
    while(ci.more()) {
        // do something
    }
}
```

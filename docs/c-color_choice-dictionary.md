# C-color Choice Dictionary

A choice dictionary is a bitset containing n elements that supports reading and setting a field of bits to a color in constant time and additionally a so-called _choice_ operation that returns the position of an arbitrary index of a field that has a chosen color other than 0 in constant time.

## Operations

- get(i): Returns the color at index i.
- insert(i, c): Set the color at index i to c.
- choice(c): Returns an arbitrary index that is set to the color c.

## Efficiency

- Time: O(1) (all operations)
- Space: O(n + n/w + 3n/(w^2))

## Example

```cpp
CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(100, 4); // The 2nd parameter defines the amount of colors including 0

cd->insert(0, 1); // Indexing beginns with 0
cd->insert(10, 1);
cd->insert(72, 1);
cd->insert(31, 1);
cd->insert(17, 2);
cd->insert(41, 2);
cd->insert(0, 3); // This overrides the previously inserted color
cd->insert(10, 0);

cd.get(0); // Returns 3
cd.get(10); // Returns 0
cd.get(31); // Returns 1
cd.get(17); // Returns 2

cd.choice(1); // May return 31, or 72
cd.choice(2); // May return 17, or 41

delete cd;
```

<br>

# Iterator

An Iterator is used to iterate over all fields set to a specific color within a choice dictionary.
It supports the so-called _more_ operation that returns true if the choice dictionary contains more bits of that color, aswell as the _next_ operation that returns the index of the next field.

## Operations

- setColor(c): Sets the color the iterate should use.
- init: Initializes the Iterator.
- more: Returns true if there are more fields with color c.
- next: Returns the next arbitrary position of a field that is set to c.

## Efficiency

- Time: O(1) (all operations)

## Example

```cpp
CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(100, 4); // The 2nd parameter defines the amount of colors including 0
ChoiceDictionaryIterator* it = new ChoiceDictionaryIterator(cd);

cd->insert(0, 1);
cd->insert(10, 1);
cd->insert(72, 1);
cd->insert(31, 1);
cd->insert(17, 2);
cd->insert(41, 2);
cd->insert(0, 3);
cd->insert(10, 0);

it->setColor(1);
it->init();

while (it->more()) { // Returns true if more fields are of color 1
    it->next(); //  May return the next arbitrary position 31, or 72
}

it->setColor(2);
it->init();

while (it->more()) { // Returns true if more fields are of color 1
    it->next(); //  May return the next arbitrary position 17, or 41
}

delete it;
delete cd;
```

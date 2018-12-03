# Space Efficient Algorithms 
[![Build Status](https://travis-ci.org/thm-mni-ii/sea.svg?branch=master)](https://travis-ci.org/thm-mni-ii/sea) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Coverage Status](https://coveralls.io/repos/github/thm-mni-ii/sea/badge.svg?branch=master)](https://coveralls.io/github/thm-mni-ii/sea?branch=master) [![Quality Gate](https://sonarcloud.io/api/project_badges/measure?project=thm-mni-ii_sea&metric=alert_status)](https://sonarcloud.io/dashboard?id=thm-mni-ii_sea) [![Reliability](https://sonarcloud.io/api/project_badges/measure?project=thm-mni-ii_sea&metric=reliability_rating)](https://sonarcloud.io/dashboard?id=thm-mni-ii_sea)

SEA is a project to research and implement an open C++ library for Space-Efficient (Graph) Algorithms (SEA).
Besides the running time of algorithms, their space requirements cause a problem if dealing with huge
data sets or executing them on tiny devices where memory is heavily limited. Therefore, we want to provide algorithms and data structures to tackle this problem by treating space as a scarce resource.

## Table of Contents
* [Project Structure](#project-structure)
* [Build](#build)
* [Algorithms and Data Structures](#algorithms-and-data-structures)
    * [Algorithms](#algorithms)
    * [Data Structures](#data-structures)
* [Research](#research)
* [Licence](#licence)
* [Acknowledgments](#acknowledgments)

## Project Structure
```bash
.
├── CMakeLists.txt  # CMake build script
├── LICENSE         # Licence description
├── README.md       # You are reading this file now
├── third-party     # Third party libraries
├── include         # The library's header files (*.h)
├── src             # The library's source files (*.cpp)
├── src-view        # The source files for the visualization (*.cpp)
├── test            # The test files
├── lib             # The library files
└── bin             # Executable files to test the project
```

## Build
1. Install CMake and a C++ compiler for your specific operating system.
2. Build a make file for your system by using CMake -> `cmake .`
3. Build the artifacts by executing make -> `make`

Now, the `include` folder contains the necessary header files and the `lib` folder contains the build library.

If you encounter any bugs, missing or misleading documentation, do not hesitate to create an [issue ticket](https://github.com/thm-mni-ii/sea/issues/new).

## Using the library
- Copy the `include/sealib` folder into your own project's include path.
- Copy the `lib/libsealib.so` file into your own project's library path. Make sure that the environment variable `LD_LIBRARY_PATH` also points there, or else the shared library won't be found.

### Simple example
This example shows how to use the contents of the library.

```cpp
#include <sealib/_types.h>
#include <sealib/dictionary/choicedictionary.h>
#include <sealib/iterator/dfs.h>
#include <sealib/graph/graphcreator.h>

bool reachable(uint a, uint b) {
    bool ret = false;
    DirectedGraph g = GraphCreator::createRandomFixed(100, 30);
    ChoiceDictionary started(100);
    ChoiceDictionary done(100);
    DFS::nloglognBitDFS(&g,
                        [&](uint u) {
                            started.insert(u);
                            if (u == b && started.get(a) && !done.get(a)) {
                                ret = true;
                            }
                        },
                        DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                        [&](uint u) { done.insert(u); });
    return ret;
}
```

Compile with:
```sh
export LD_LIBRARY_PATH=<library-path>
clang++ -I<include-path> -L<libary-path> -o reachable reachable.cpp -lsealib
```


## Algorithms and Data Structures
This section gives you a brief overview over the implemented algorithms and data structures. For a detailed documentation click on a algorithm (coming soon).
For some data structures and algorithms we also provide a folklore implementation that we use to check the space efficient algorithm and to run runtime and space consumption tests.

### Algorithms
* Depth First Search
    * A folklore depth first search that runs in O(n + m) time, using O((n + m) log n) bits.
    * A depth first search that runs in O((n + m) log n) time, using O((log(3)+ε) n) bits. [Read more ...](docs/n-bit-dfs.md) 
    * A depth first search that runs in O(n + m) time, using O(n log(log(n))) bits. [Read more ...](docs/nloglogn-bit-dfs.md) 
    * A depth first search of an undirected graph that runs in O(n + m) time, using O(n + m) bits. [Read more ...](docs/nplusm-bit-dfs.md)
    

### Data Structures
* Graph(G = {V, E}): A adjacency list graph representation that occupies O((n + m) log n) bits.
* Bitset(n): A bitset of n bits that supports access in O(1) time and occupies O(n) bits.
* [ChoiceDictionary](docs/choice-dictionary.md): A bitset that supports a choice operation in O(1) time that returns the position of a bit set to 1. The total representation requires O(n + n/w + 3n/(w^2)) bits of memory.
* RankSelect(n): A bit sequence that supports the operations *rank(k)* and *select(k)* in O(1) time and occupies O(n) bits. *rank(k)* returns the number of set bits up to index k, and *select(k)* returns the index of the k-th set bit.
* StaticSpaceStorage(n): A sequence of n bit packs of variable size that can be accessed in O(1) time and occupies O(n + N) bits. N is the total usable size of the static-space storage.


## Research
We publish most of our research on arXiv.org.

* [Extra Space during Initialization of Succinct Data Structures and Dynamical Initializable Arrays](https://arxiv.org/abs/1803.09675)
* [Linear-Time In-Place DFS and BFS in the Restore Model](https://arxiv.org/abs/1803.04282)

## License
Licensed under the GNU General Public License version 3. For detailed license information look inside the [LICENSE](LICENSE) file.

## Acknowledgments
Funded by the Deutsche Forschungsgemeinschaft (DFG, German Research Foundation) – 379157101.
For more details visit [http://gepris.dfg.de/gepris/projekt/379157101?language=en]
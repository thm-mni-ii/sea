Space Efficient Algorithms [![Build Status](https://travis-ci.org/thm-mni-ii/sea.svg?branch=master)](https://travis-ci.org/thm-mni-ii/sea) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Coverage Status](https://coveralls.io/repos/github/thm-mni-ii/sea/badge.svg?branch=master)](https://coveralls.io/github/thm-mni-ii/sea?branch=master)
===

SEA is a project to research and implement an open C++ library for Space-Efficient (Graph) Algorithms (SEA).
Besides the running time of algorithms, their space requirements cause a problem if dealing with huge
data sets or executing them on tiny devices where memory is heavily limited. Therefore, we want to provide algorithms and data structures to tackle this problem by treating space as a scarse ressourse.

# Table of Content
* [Algorithms and Data Structures](#algorithms-and-data-structures)
    * [Algorithms](#algorithms)
    * [Data Structures](#data-structures)
* [Build](#build)
* [Project Structure](#project-structure)
* [Research](#research)
* [Licence](#licence)
* [Acknowledgments](#acknowledgments)

# Algorithms and Data Structures
This section gives you a brief overview over the implemented algorithms and data structures. For a detailed documentation click on the algorithm (comming soon).
For some data structure and algorithm we also provide a folklore implementation that we use to check the space efficient algorithm and to run runtime and space consumption tests.

## Algorithms
* Depth First Search
    * A folklore depth first search that runs in O(n + m) time, using O((n + m) log n) bits.
    * [Details](docs/n-bit-dfs.md) A depth first search that runs in O((n + m) log n) time, using O((log(3)+ε) n) bits.
    

## Data Structures
* Bitset(n): A bitset of n bits that supports access in O(1) time and occupies O(n) bits.
* Graph(G = {V, E}): A adjacency list graph representation that occupies O((n + m) log n) bits.
* [ChoiceDictionary](docs/choice-dictionary.md): A bitset that supports a choice operation in O(1) time that returns the position of a bit set to 1. (Currently occupying O(n \log n) bits.)

# Build
1. Install CMake and a C++ compiler for your specific operating system.
2. Build a make file for your system by using CMake -> `cmake .`
3. Build the artifacts by executing make -> `make`

Now, the `include` folder contains the nessasary header files and the `lib` folder contains the build library.

If you encounter any bugs, missing or misleading documentation, do not hesitate to create an [issue ticket](https://github.com/thm-mni-ii/sea/issues/new).

# Project Structure
```bash
.
├── CMakeLists.txt  # CMake build script
├── LICENSE         # Licence descriptionö
├── README.md       # You are reading this file now
├── third-party     # Third party libraries
├── include         # The libraries header files (*.h)
├── src             # The libraries source files (*.cpp)
├── src-view        # The source files for the visualization (*.cpp)
├── test            # The test files
├── lib             # The library files
└── bin             # Executable files to test the project
```

# Research
We publish most of our research on arXiv.org.

* [Extra Space during Initialization of Succinct Data Structures and Dynamical Initializable Arrays](https://arxiv.org/abs/1803.09675)
* [Linear-Time In-Place DFS and BFS in the Restore Model](https://arxiv.org/abs/1803.04282)

# License
Licensed under the GNU General Public License version 3. For detailed license information look inside the [LICENSE](LICENSE) file.

# Acknowledgments
Funded by the Deutsche Forschungsgemeinschaft (DFG, German Research Foundation) – 379157101.
For more details visit [http://gepris.dfg.de/gepris/projekt/379157101?language=en]
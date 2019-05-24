Project guidelines
===
### Preliminary
In case of a failed build, check the Travis CI output of your pull request and fix any reported errors.

To prevent annoying linter errors, download the [cpplint Python script](https://github.com/google/styleguide/blob/gh-pages/cpplint/cpplint.py) and execute it locally. Then fix all the errors and submit your pull request.

### Headers and sources
We usually split an implementation into a header and a source file. The header file contains the *declarations* of classes and methods while the source file contains their *definitions*.
- A header file must reside in `include/sealib/`. We use four special subdirectories to get a better overview of the library:
    - Graph classes and utilities: `include/sealib/graph`
    - Algorithms for graph traversal/iteration: `include/sealib/iterator`
    - General data structures: `include/sealib/collection`
    - Dictionaries: `include/sealib/dictionary`
- A source file must reside in `src/`. There, you have some liberty in creating subdirectories for different algorithms and data structures.

### Namespace
- Any class must be declared in the namespace `Sealib`.
- Do not use `using` or `using namespace` in a header file.

### Special member functions
Special member functions are: *destructor*, *copy constructor*, *copy assignment*, *move constructor* and *move assignment*.
- Always follow the [**Rule of Zero**](http://web.archive.org/web/20170705080311/https://rmf.io/cxx11/rule-of-zero). Do not explicitly define any special member function, instead use standard-library classes like `std::vector` and `std::unique_ptr`/`std::shared_ptr` to handle ownership.
    - Exception: If you define an abstract base class (= interface), define all five special members with `protected` visibility. In addition, make the destructor `virtual`. (see `include/sealib/graph/graph.h`)

### Parameters
- An *input parameter* of type `T` should be defined as `T const &`. Small/primitive types can be passed by value.
- An *output parameter* of type `T` should be defined as `T *`. (But prefer to simply return an object of type `T`.)

### Unit tests
We use the Google Test testing framework. Take a look at [their documentation](https://github.com/google/googletest/blob/master/googletest/docs/primer.md#basic-concepts) to get started.

#include <sealib/_types.h>
#include <sealib/graph/graphcreator.h>

#include <cstdio>
#include <cstring>

#include "./test_algorithms.h"
#include "./test_visual.h"

int main(int argc, char **argv) {
    if (argc < 6) {
        printf("Usage: bin/main <program> <file1> <file2> <from> <to>\n");
        return 1;
    }
    return Sealib::AlgorithmComparison::launch(
        argv[1], argv[2], argv[3],
        static_cast<uint64_t>(strtod(argv[4], nullptr)),
        static_cast<uint64_t>(strtod(argv[5], nullptr)));
}



#include <ctime>
#include <iostream>
#include <include/sealib/graph.h>
#include <include/sealib/graphio.h>
#include <include/sealib/rankselect.h>
#include <include/sealib/graphalgorithms.h>

void testHierholzer() {
    clock_t start = clock();
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(15,45);
    clock_t end = clock();
    std::cout << "GENERATION: " << (end - start)/CLOCKS_PER_SEC << "s" << std::endl;
    start = clock();
    Sealib::TrailStructure **ts = Sealib::GraphAlgorithms::hierholzer(g);
    end = clock();
    std::cout << "HIERHOLZER: " << (end - start)/CLOCKS_PER_SEC << "s" << std::endl;
    start = clock();
    std::cout << Sealib::GraphIO::stringFromTrail(g, ts) << std::endl;
    end = clock();
    std::cout << "STRING: " << (end - start)/CLOCKS_PER_SEC << "s" << std::endl;
}

void testRankStructure() {
    boost::dynamic_bitset<> bits(2);
    Sealib::SimpleRankSelect simple(bits);

    std::cout << simple.select(0) << std::endl;
    std::cout << simple.rank(0) << std::endl;

    std::cout << simple.select(1) << std::endl;
    std::cout << simple.rank(1) << std::endl;

    std::cout << simple.select(1241241) << std::endl;
    std::cout << simple.rank(1241241) << std::endl;

    Sealib::RankSelect rankSelect(bits);
    std::cout << bits.size() << std::endl;
    std::cout << rankSelect.select(0) << std::endl;
    std::cout << rankSelect.rank(0) << std::endl;

    std::cout << rankSelect.select(1) << std::endl;
    std::cout << rankSelect.rank(1) << std::endl;

    std::cout << rankSelect.select(2) << std::endl;
    std::cout << rankSelect.rank(2) << std::endl;

    std::cout << rankSelect.select(1241241) << std::endl;
    std::cout << rankSelect.rank(1241241) << std::endl;
}

int main() {
    testRankStructure();
}


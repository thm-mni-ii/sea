#include <ctime>
#include <include/sealib/graphio.h>
#include <iostream>
#include <include/sealib/graphalgorithms.h>
#include <include/sealib/graphcreator.h>

void testHierholzer() {
    clock_t start = clock();
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(1500, 45000);
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

clock_t profileHierholzer(unsigned int m, unsigned int n, bool getMatches = false) {
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(m, n);

    if(getMatches) {
        clock_t start = clock();
        Sealib::TrailStructure **ts = Sealib::GraphAlgorithms::hierholzer(g);
        for(unsigned int i = 0; i < g->getOrder(); i++) {
            for(unsigned int j = 0; j < ts[i]->getDegree(); j++) {
                ts[i]->getMatched(j);
            }
        }
        clock_t end = clock();
        delete(g);
        return (end - start);
    } else {
        clock_t start = clock();
        Sealib::GraphAlgorithms::hierholzer(g);
        clock_t end = clock();
        delete(g);
        return (end - start);
    }
}

clock_t profileHierholzerOverhead(unsigned int m, unsigned int n) {
    clock_t start = clock();
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(m, n);
    delete(g);
    clock_t end = clock();
    return (end - start);
}

clock_t profileHierholzerSimple(unsigned int m, unsigned int n, bool getMatches = false) {
    Sealib::Graph *g = Sealib::GraphIO::randomEulerianSealibGraph(m, n);

    if(getMatches) {
        clock_t start = clock();
        Sealib::SimpleTrail ts = Sealib::GraphAlgorithms::hierholzerSimple(g);
        for(auto a: ts.getTrail()) {
            std::get<0>(a);
            std::get<1>(a);
        }
        clock_t end = clock();
        delete(g);
        return (end - start);
    } else {
        clock_t start = clock();
        Sealib::GraphAlgorithms::hierholzerSimple(g);
        clock_t end = clock();
        delete(g);
        return (end - start);
    }
}

int main(int argc, char *argv[]) {
    std::string mPrefix("m=");
    std::string nPrefix("m=");
    std::string typePrefix("type=");
    std::string traversePrefix("traverse=");
    int m = 100;
    int n = 1000;
    bool overhead = false;
    bool simple = false;
    bool traverse = false;
    for(int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if (!arg.compare(0, mPrefix.size(), mPrefix)) {
            m = atoi(arg.substr(mPrefix.size()).c_str());
        } else if (!arg.compare(0, nPrefix.size(), nPrefix)) {
            n = atoi(arg.substr(nPrefix.size()).c_str());
        } else if (!arg.compare(0, typePrefix.size(), typePrefix)) {
            std::string subString = arg.substr(typePrefix.size());
            simple = !subString.compare("simple");
        } else if (!arg.compare(0, typePrefix.size(), typePrefix)) {
            std::string subString = arg.substr(typePrefix.size());
            overhead = !subString.compare("overhead");
        } else if (!arg.compare(0, traversePrefix.size(), traversePrefix)) {
            std::string subString = arg.substr(traversePrefix.size());
            traverse = !subString.compare("true");
        }
    }
    if(overhead) {
        std::cout << profileHierholzerOverhead(m,n);
    } else if(simple) {
        std::cout << profileHierholzerSimple(m,n, traverse);
    } else {
        std::cout << profileHierholzer(m,n, traverse);
    }
}

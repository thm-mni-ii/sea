#include <iostream>
#include <chrono>
#include <stack>
#include <random>
#include <string>
#include <random>
#include <memory>
#include <ctime>
#include <cmath>
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"
#include "sealib/graphrepresentations.h"
#include "sealib/compactgraph.h"
#include "sealib/runtimetest.h"

using std::cout;
using namespace std;
using Sealib::Compactgraph;
using Sealib::DFS;
using Sealib::Graphrepresentations;

#define VERY_SLIM   ([](double n) { return (5. * n) / (n * (n - 1.)); })
#define SLIM        ([](double n) { return (n * std::log2(n)) / (n * (n - 1.)); })
#define MODERATE    ([](double n) { return (n * std::sqrt(n)) / (n * (n - 1.)); })
#define DENSE       ([](double n) { return (std::pow(n, 2.) / std::log2(n)) / (n * (n - 1.)); })

unsigned int controllSum = (2 * (1 + 2 + 3 + 4 + 5));
stack<unsigned int> controllStack;

void preTwo(unsigned int a) {
    std::cout << "PRE-PROCESS(" << a << ")" << std::endl;
    controllSum = controllSum - a;
    controllStack.push(a);
    std::cout << "newSum: " << controllSum << std::endl;
}

void postTwo(unsigned int a) {
    std::cout << "POST-PROCESS(" << a << ")" << std::endl;
    controllSum = controllSum - a;
    unsigned int ex = controllStack.top();
    controllStack.pop();
    if (ex != a) {
        std::cout << "panic expected " << a << " on stack, got " << ex << std::endl;
        exit(100);
    }
    std::cout << "newSum: " << controllSum << std::endl;
}

unsigned int controllSumZero = (2 * (1 + 2 + 3 + 4));
stack<unsigned int> controllStack2;

void preZero(unsigned int a) {
    std::cout << "PRE-PROCESS(" << a << ")" << std::endl;
    controllStack2.push(a);
    controllSumZero = controllSumZero - a;
    std::cout << "newSum: " << controllSum << std::endl;
}

void postZero(unsigned int a) {
    std::cout << "POST-PROCESS(" << a << ")" << std::endl;
    unsigned int ex = controllStack2.top();
    controllSumZero = controllSumZero - a;
    controllStack2.pop();
    if (ex != a) {
        std::cout << "panic expected " << a << " on stack, got " << ex << std::endl;
        exit(100);
    }
    std::cout << "newSum: " << controllSum << std::endl;
}

std::mt19937_64 gen;

void dummy(uint v) {}
void dummy2(uint u, uint v) {}

void runTests(double(*p)(double), std::string filename) {
    RuntimeTest test;
    for (unsigned int i = 1; i <= 1; ++i) {
        double n = 20000 * i;
        for (unsigned int j = 0; j <= 10; ++j) {
            std::unique_ptr<Compactgraph> g(Graphrepresentations::generateGilbertGraph(n, p(n), &gen));
            test.runTest([&g]() { DFS::standardDFS(g.get(), dummy, dummy2, dummy2, dummy); }, n, n * (n - 1) * p(n));
        }
    }
    //test.saveCSV("~/" + filename);
    test.printResults();
}

double verySlim(double n){
    return (5. * n) / (n * (n - 1.));
}

int main() {
    gen.seed(std::mt19937_64::default_seed);
    runTests(verySlim, "verySlim.csv");
    // runTests(SLIM, "slim.csv");
    // runTests(MODERATE, "moderate.csv");
    // runTests(DENSE, "thick.csv");
    return 0;

//    // Graph in the swapped begin pointer representation
//    auto *graph = new unsigned int[19]{ 5, 9, 7, 9, 9, 7, 12, 1, 17, 2, 12, 14, 3, 14, 4, 12, 17, 5, 14 };
//    auto *graph3 = new unsigned int[19]{ /*n*/ 3, 7, 5, 5, /*m*/ 6, 1, 9, 2, 9, 3, 7 };
//    auto *graph4 = new unsigned int[10]{3, 7,8,5, 5, 1,9, 2, 3,7};
//    auto *graph = new unsigned int[15]{5, 9,10,7,13,7, 8, 1,12, 2, 3,9, 4, 5,12};
//
//    auto *graph = new unsigned int[9]{4, 8,2,3,3, 3, 1,2, 4};
//
//    // Grade zero
//    auto *graph = new unsigned int[9]{4, 2, 2, 3, 4, 3, 1, 3, 4};
//
//    auto wcts = std::chrono::system_clock::now();

//    std::mt19937_64 rng;
//    rng.seed(std::mt19937_64::default_seed);

    //uint* graph = Graphrepresentations::generateRawGilbertGraph(500, 0.5, &rng); // TODO(Andrej)

    //Graphrepresentations::standardToBeginpointer(graph);
    //Graphrepresentations::swapRepresentation(graph);

//    DFS::runLinearTimeInplaceDFS(graph, [](uint v) { std::cout << "" << std::endl; },
//                                 [](uint v) { std::cout << "" << std::endl; }, 1);
//
//    std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);
//    std::cout << "Finished in " << wctduration.count() << " seconds [Wall Clock]" << std::endl;
}

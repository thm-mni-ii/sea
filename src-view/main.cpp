#include <iostream>
#include <chrono>
#include <stack>
#include <random>
#include <string>
#include <sstream>
#include <memory>
#include <ctime>
#include <cmath>
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"
#include "sealib/graphrepresentations.h"
#include "sealib/compactgraph.h"
#include "sealib/runtimetest.h"
#include "sealib/_types.h"

using std::cout;
using std::stack;
using Sealib::Compactgraph;
using Sealib::DFS;
using Sealib::Graphrepresentations;

#define VERY_SPARSE ([](double n) { return (5. * n) / (n * (n - 1.)); })
#define SPARSE      ([](double n) { return (n * std::log2(n)) / (n * (n - 1.)); })
#define MODERATE    ([](double n) { return (n * std::sqrt(n)) / (n * (n - 1.)); })
#define DENSE       ([](double n) { return (std::pow(n, 2.) / std::log2(n)) / (n * (n - 1.)); })

uint32_t controllSum = (2 * (1 + 2 + 3 + 4 + 5));
stack<uint32_t> controllStack;

void preTwo(uint32_t a) {
    std::cout << "PRE-PROCESS(" << a << ")" << std::endl;
    controllSum = controllSum - a;
    controllStack.push(a);
    std::cout << "newSum: " << controllSum << std::endl;
}

void postTwo(uint32_t a) {
    std::cout << "POST-PROCESS(" << a << ")" << std::endl;
    controllSum = controllSum - a;
    uint32_t ex = controllStack.top();
    controllStack.pop();
    if (ex != a) {
        std::cout << "panic expected " << a << " on stack, got " << ex << std::endl;
        exit(100);
    }
    std::cout << "newSum: " << controllSum << std::endl;
}

uint32_t controllSumZero = (2 * (1 + 2 + 3 + 4));
stack<uint32_t> controllStack2;

void preZero(uint32_t a) {
    std::cout << "PRE-PROCESS(" << a << ")" << std::endl;
    controllStack2.push(a);
    controllSumZero = controllSumZero - a;
    std::cout << "newSum: " << controllSum << std::endl;
}

void postZero(uint32_t a) {
    std::cout << "POST-PROCESS(" << a << ")" << std::endl;
    uint32_t ex = controllStack2.top();
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
    RuntimeTest test1, test2;
    for (uint32_t i = 1; i <= 2; ++i) {
        double n = 20000 * i;
        for (uint32_t j = 0; j < 2; ++j) {
            double pN = p(n);
            double m =  n * (n-1) * pN;
            auto A = Graphrepresentations::generateRawGilbertGraph(n, pN, &gen);
            std::unique_ptr<Compactgraph> g(new Compactgraph(A));
            test1.runTest([&g]() { DFS::standardDFS(g.get(), dummy,
                                                    dummy2, dummy2, dummy); }, n, m);
            test2.runTest([&A]() {
                Graphrepresentations::standardToBeginpointer(A);
                Graphrepresentations::swapRepresentation(A);
                DFS::runLinearTimeInplaceDFS(A, dummy, dummy, 1);
            }, n, m);
        }
    }

    test1.printResults();
    test2.printResults();
}

void runTest(uint n, uint (*fm)(uint n)) {
    RuntimeTest test1, test2;
    auto _n = n;
    for (uint i = 1; i <= 1; i++) {
        for (uint32_t j = 0; j < 1; ++j) {
            auto A = Graphrepresentations::fastGraphGeneration(_n, fm(_n));
            std::unique_ptr<Compactgraph> g(new Compactgraph(A));
            test1.runTest([&g]() {
                DFS::standardDFS(g.get(), dummy, dummy2, dummy2, dummy);
            }, _n, fm(_n));
//            test2.runTest([&A]() {
//                Graphrepresentations::standardToBeginpointer(A);
//                Graphrepresentations::swapRepresentation(A);
//                DFS::runLinearTimeInplaceDFS(A, dummy, dummy, 1);
//            }, _n, fm(_n));
        }
        _n = n * 2 * i;
    }
    test1.printResults();
    test2.printResults();
}

int main() {
    return 0;
}

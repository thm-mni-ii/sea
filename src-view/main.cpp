#include <sealibvisual/tikznode.h>
#include <sealibvisual/tikzarray.h>
#include <sealibvisual/tikzdocument.h>
#include <sealibvisual/tikzpicture.h>
#include <sealibvisual/tikzgenerator.h>
#include <sealibvisual/tikzfigure.h>
#include <sealib/graphcreator.h>
#include <sealib/dfs.h>
#include <sealib/graphrepresentations.h>
#include <sealib/basicgraph.h>
#include <sealib/compactgraph.h>
#include <sealib/runtimetest.h>
#include <sealib/_types.h>
#include <iostream>
#include <chrono>
#include <stack>
#include <random>
#include <string>
#include <sstream>
#include <memory>
#include <ctime>
#include <cmath>
#include <include/sealibvisual/tikzgraph.h>

using std::cout;
using std::stack;
using Sealib::BasicGraph;
using Sealib::Compactgraph;
using Sealib::DFS;
using Sealib::Graphrepresentations;

#define VERY_SPARSE ([](double n) { return (5. * n) / (n * (n - 1.)); })
#define SPARSE      ([](double n) { return (n * std::log2(n)) / (n * (n - 1.)); })
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
    RuntimeTest test1, test2;
    for (unsigned int i = 1; i <= 2; ++i) {
        double n = 20000 * i;
        for (unsigned int j = 0; j < 2; ++j) {
            double pN = p(n);
            double m = n * (n - 1) * pN;
            auto A = Graphrepresentations::generateRawGilbertGraph(n, pN, &gen);
            std::unique_ptr<Compactgraph> g(new Compactgraph(A));
            test1.runTest([&g]() {
              DFS::standardDFS(g.get(), dummy,
                               dummy2, dummy2, dummy);
            }, n, m);
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
        for (unsigned int j = 0; j < 1; ++j) {
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

void tikz_example() {
    std::vector<std::string> numbers1(10);
    for (unsigned int i = 0; i < 10; i++) {
        numbers1[i] = std::to_string(i * 45);
    }

    Sealib::Bitset<unsigned char> bits(10);
    bits[4] = 1;

    std::shared_ptr<SealibVisual::TikzElement> tikzNode =
        SealibVisual::TikzGenerator::generateTikzElement<Sealib::Bitset<unsigned char>>(bits);

    SealibVisual::TikzDocument doc("out.tex", "matrix, backgrounds");
    doc.add(tikzNode);
    doc.close();
}

void tikz_example2() {
    unsigned int order = 4;
    auto **adj_mtrx = (unsigned int **) malloc(sizeof(unsigned int) * 4 * 4);
    adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0};
    adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
    adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};

    Sealib::BasicGraph bg =
        *Sealib::GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);

    std::shared_ptr<SealibVisual::TikzElement> vg =
         SealibVisual::TikzGenerator::generateTikzElement(bg);

    std::shared_ptr<SealibVisual::TikzPicture>
        pic(new SealibVisual::TikzPicture("spring layout, node distance=100pt, scale=1"));

    pic->add(vg);

    std::shared_ptr<SealibVisual::TikzFigure> fig(new SealibVisual::TikzFigure("A caption.", pic));

    SealibVisual::TikzDocument doc("out.tex", "matrix, backgrounds, graphdrawing", "force", true);
    doc.add(fig);

    for(auto &e: ((SealibVisual::TikzGraph*) (vg.get()))->getEdges()) {
        e.second.setOptions("--, color=green, line width=8");
    }
    doc.add(fig);

    std::vector<std::string> numbers1(10);
    for (unsigned int i = 0; i < 10; i++) {
        numbers1[i] = std::to_string(i * 45);
    }

    Sealib::Bitset<unsigned char> bits(10);
    bits[4] = 1;

    std::shared_ptr<SealibVisual::TikzElement> tikzNode =
        SealibVisual::TikzGenerator::generateTikzElement<Sealib::Bitset<unsigned char>>(bits);
    pic->add(tikzNode);

    doc.add(fig);

    doc.close();
}

int main() {
    tikz_example2();
}

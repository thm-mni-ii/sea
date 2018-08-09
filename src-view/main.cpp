#include <iostream>
#include <chrono>
#include "sealib/graphcreator.h"
#include "sealib/dfs.h"
#include "sealib/graphrepresentations.h"
#include "sealib/compactgraph.h"
#include <stack>
#include <random>
#include <iostream>
#include <string>
#include <random>
#include <memory>
#include <ctime>
#include <cmath>
#include "sealib/graphrepresentations.h"
#include "sealib/compactgraph.h"
#include "sealib/dfs.h"
#include "sealib/runtimetest.h"

using std::cout;
using namespace std;
using Sealib::DFS;
using Sealib::Graphrepresentations;

unsigned int controllSum = (2 * (1 + 2 + 3 + 4 + 5));
stack <unsigned int> controllStack;
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
stack <unsigned int> controllStack2;
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

int main() {
//    // Graph in the swapped begin pointer representation
//    auto *graph = new unsigned int[19]{ 5, 9, 7, 9, 9, 7, 12, 1, 17, 2, 12, 14, 3, 14, 4, 12, 17, 5, 14 };
//    auto *graph3 = new unsigned int[19]{ /*n*/ 3, 7, 5, 5, /*m*/ 6, 1, 9, 2, 9, 3, 7 };
//    auto *graph4 = new unsigned int[10]{3, 7,8,5, 5, 1,9, 2, 3,7};
//    auto *graph = new unsigned int[15]{5, 9,10,7,13,7, 8, 1,12, 2, 3,9, 4, 5,12};
//
//    auto *graph = new unsigned int[9]{4, 8,2,3,3, 3, 1,2, 4};
//
//    // Grade zero
 auto *graph = new unsigned int[9]{4,2,2,3,4,3,1,3,4};
//
    auto wcts = std::chrono::system_clock::now();

    std::mt19937_64 rng;
    rng.seed(std::mt19937_64::default_seed);

    //uint* graph = Graphrepresentations::generateRawGilbertGraph(500, 0.5, &rng); // TODO(Andrej)

    //Graphrepresentations::standardToBeginpointer(graph);
    //Graphrepresentations::swapRepresentation(graph);

    DFS::runLinearTimeInplaceDFS(graph, [](uint v) { std::cout << "" << std::endl; }, [](uint v) { std::cout << "" << std::endl; }, 1);

    std::chrono::duration<double> wctduration = (std::chrono::system_clock::now() - wcts);
    std::cout << "Finished in " << wctduration.count() << " seconds [Wall Clock]" << std::endl;
}

#include <sealib/_types.h>
#include <sealib/choicedictionaryiterator.h>
#include <sealib/compactgraph.h>
#include <sealib/dfs.h>
#include <sealib/graphcreator.h>
#include <sealib/graphrepresentations.h>
#include <sealib/runtimetest.h>
#include <chrono>
#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include "sealib/bitarray.h"
#include "sealibvisual/examples.h"
#include "sealibvisual/tikzarray.h"
#include "sealibvisual/tikzdocument.h"
#include "sealibvisual/tikzfigure.h"
#include "sealibvisual/tikzgenerator.h"
#include "sealibvisual/tikzgraph.h"
#include "sealibvisual/tikznode.h"
#include "sealibvisual/tikzpicture.h"
#include "sealibvisual/tikzstyle.h"

using std::cout;
using std::stack;
using namespace Sealib;  // NOLINT

using SealibVisual::TikzElement;
using SealibVisual::TikzGenerator;
using SealibVisual::TikzPicture;
using SealibVisual::TikzDocument;
using SealibVisual::TikzGraph;
using SealibVisual::TikzNode;

#define VERY_SPARSE ([](double n) { return (5. * n) / (n * (n - 1.)); })
#define SPARSE ([](double n) { return (n * std::log2(n)) / (n * (n - 1.)); })
#define MODERATE ([](double n) { return (n * std::sqrt(n)) / (n * (n - 1.)); })
#define DENSE \
  ([](double n) { return (std::pow(n, 2.) / std::log2(n)) / (n * (n - 1.)); })

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

void runTests(double (*p)(double), std::string filename) {
  RuntimeTest test1, test2;
  for (unsigned int i = 1; i <= 2; ++i) {
    double n = 20000 * i;
    for (unsigned int j = 0; j < 2; ++j) {
      double pN = p(n);
      double m = n * (n - 1) * pN;
      auto A = Graphrepresentations::generateRawGilbertGraph(n, pN, &gen);
      std::unique_ptr<Compactgraph> g(new Compactgraph(A));
      test1.runTest(
          [&g]() { DFS::standardDFS(g.get(), dummy, dummy2, dummy2, dummy); },
          n, m);
      test2.runTest(
          [&A]() {
            Graphrepresentations::standardToBeginpointer(A);
            Graphrepresentations::swapRepresentation(A);
            DFS::runLinearTimeInplaceDFS(A, dummy, dummy, 1);
          },
          n, m);
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
      test1.runTest(
          [&g]() { DFS::standardDFS(g.get(), dummy, dummy2, dummy2, dummy); },
          _n, fm(_n));
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

void runtime_iterate() {
  std::random_device rnd;
  RuntimeTest t1, t2, t3, t4;
  for (uint64_t n = 1e6; n <= 1e7; n += 10000) {
    std::uniform_int_distribution<uint64_t> dist(0, n - 1);
    uint64_t p = n / 100;
    std::vector<uint64_t> pos(p);
    for (uint64_t a; a < p; a++) pos.emplace_back(dist(rnd));
    // CompactArray c(n, 2);
    {
      ChoiceDictionary cd(n);
      for (uint64_t a : pos) cd.insert(a);
      ChoiceDictionaryIterator i(&cd);
      i.init();
      t1.runTest(
          [&i]() {
            std::vector<unsigned long> r;
            while (i.more()) r.push_back(i.next());
          },
          n, 0);
    }
    {
      std::unique_ptr<char[]> l(new char[n]);
      for (uint64_t a : pos) l[a] = 1;
      t2.runTest(
          [&l, n]() {
            std::vector<uint64_t> r;
            for (uint64_t a = 0; a < n; a++) {
              if (l[a] == 1) {
                r.push_back(a);
              }
            }
          },
          n, 0);
    }
    /*t3.runTest(
        [&c, n]() {
          std::vector<uint64_t> r;
          for (uint64_t a = 0; a < n; a++) {
            if (c.get(a) == 1) {
              r.push_back(a);
            }
          }
        },
        n, 0);*/
    {
      BitArray b(n, 2);
      for (uint64_t a : pos) b.insert(a, 1);
      t4.runTest(
          [&b, n]() {
            std::vector<uint64_t> r;
            for (uint64_t a = 0; a < n; a++) {
              if (b.get(a) == 1) {
                r.push_back(a);
              }
            }
          },
          n, 0);
    }
  }
  t1.saveCSV("iterate-cd.csv");
  t2.saveCSV("iterate-array.csv");
  // t3.saveCSV("iterate-compactarray.csv");
  t4.saveCSV("iterate-bitarray.csv");
}

void runtime_dfs() {
  RuntimeTest t1;
  for (uint64_t n = 1e5; n <= 1e6; n += 10000) {
    Graph *g = GraphCreator::createRandomFixed(n, 5);
    t1.runTest([g]() { DFS::nloglognBitDFS(g); }, n, 0);
  }
  t1.saveCSV("nloglogn-ca.csv");
}

void tikz_example() {
  std::vector<std::string> numbers1(10);
  for (unsigned int i = 0; i < 10; i++) {
    numbers1[i] = std::to_string(i * 45);
  }

  Sealib::Bitset<unsigned char> bits(10);
  bits[4] = 1;

  std::shared_ptr<SealibVisual::TikzElement> tikzNode =
      SealibVisual::TikzGenerator::generateTikzElement(bits);

  SealibVisual::TikzDocument doc("out.tex", "matrix, backgrounds");
  doc.add(tikzNode);
  doc.close();
}

void tikz_example2() {
  unsigned int order = 4;
  auto **adj_mtrx = (unsigned int **)malloc(sizeof(unsigned int) * 4 * 4);
  adj_mtrx[0] = new unsigned int[order]{0, 1, 0, 1};
  adj_mtrx[1] = new unsigned int[order]{1, 0, 1, 0};
  adj_mtrx[2] = new unsigned int[order]{0, 1, 0, 1};
  adj_mtrx[3] = new unsigned int[order]{1, 0, 1, 0};

  Sealib::BasicGraph bg =
      *Sealib::GraphCreator::createGraphFromAdjacencyMatrix(adj_mtrx, order);
  std::shared_ptr<SealibVisual::TikzElement> vg =
      SealibVisual::TikzGenerator::generateTikzElement(bg);

  std::shared_ptr<SealibVisual::TikzPicture> pic(new SealibVisual::TikzPicture(
      "spring layout, node distance=100pt, scale=1"));

  pic->add(vg);

  std::shared_ptr<SealibVisual::TikzFigure> fig(
      new SealibVisual::TikzFigure("A caption.", pic));

  SealibVisual::TikzDocument doc("out.tex", "matrix, backgrounds, graphdrawing",
                                 "force", true);
  doc.add(fig);

  for (auto &e : ((SealibVisual::TikzGraph *)(vg.get()))->getEdges()) {
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
      SealibVisual::TikzGenerator::generateTikzElement(bits);
  pic->add(tikzNode);

  doc.add(fig);

  doc.close();
}

void tikz_example3() {
  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(10, 4);
  std::shared_ptr<TikzGraph> vg = TikzGenerator::generateTikzElement(*g);
  TikzGraph *tg = vg.get();
  for (auto &e : tg->getEdges()) {
    e.second.setOptions("line width=1pt, color=black, arrows={->}");
  }

  Sealib::CompactArray c(10);
  std::shared_ptr<TikzPicture> vc =
      TikzGenerator::generateTikzElement(c, "color");

  std::shared_ptr<TikzPicture> pic1(new TikzPicture("spring layout"));
  std::shared_ptr<TikzPicture> pic2(new TikzPicture(""));

  pic1->add(vg);

  TikzDocument doc("out-bfs.tex", "matrix,positioning,graphdrawing",
                   "layered,force", true);
  doc.add(pic1);
  // doc.add(pic2);
  doc.add(vc);
  doc.add("\\newpage");

  tg->getNodes().at(0).setOptions("circle,draw=black,fill=gray");
  c.insert(0, 1);
  doc.add(pic1);
  doc.add(TikzGenerator::generateTikzElement(c, "color"));
  doc.add("\\newpage");

  tg->getNodes().at(0).setOptions("circle,draw=black,fill=gray");
  c.insert(0, 1);
  doc.add(pic1);
  doc.add(TikzGenerator::generateTikzElement(c, "color"));
  doc.add("\\newpage");

  tg->getNodes().at(1).setOptions("circle,draw=black,fill=gray!30");
  tg->getNodes().at(5).setOptions("circle,draw=black,fill=gray!30");
  tg->getNodes().at(7).setOptions("circle,draw=black,fill=gray!30");
  tg->getNodes().at(8).setOptions("circle,draw=black,fill=gray!30");
  c.insert(1, 2);
  c.insert(5, 2);
  c.insert(7, 2);
  c.insert(8, 2);
  doc.add(pic1);
  doc.add(TikzGenerator::generateTikzElement(c, "color"));
  doc.add("\\newpage");

  tg->getNodes().at(0).setOptions("circle,draw=black,fill=black");
  tg->getNodes().at(1).setOptions("circle,draw=black,fill=gray");
  tg->getNodes().at(5).setOptions("circle,draw=black,fill=gray");
  tg->getNodes().at(7).setOptions("circle,draw=black,fill=gray");
  tg->getNodes().at(8).setOptions("circle,draw=black,fill=gray");
  c.insert(0, 3);
  c.insert(1, 1);
  c.insert(5, 1);
  c.insert(7, 1);
  c.insert(8, 1);
  doc.add(pic1);
  doc.add(TikzGenerator::generateTikzElement(c, "color"));
  doc.add("\\newpage");

  doc.close();

  /* Convert PDF to GIF:
    convert -density 200 -delay 100 -background white -alpha remove out-bfs.pdf
    out-bfs.gif       //NOLINT
  */
}

using SealibVisual::Examples;
using SealibVisual::VisualDFS;
using SealibVisual::VisualBFS;
void tikz_exampleDFS() {
  uint n = 20;
  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 3);
  Sealib::CompactArray c(n, 3);
  VisualDFS d(g, &c, "out-dfs3.tex");
  d.run();
  delete g;
}

void tikz_exampleBFS() {
  uint n = 20;
  BasicGraph *g = Sealib::GraphCreator::createRandomFixed(n, 3);
  Sealib::CompactArray c(n, 4);
  VisualBFS b(g, &c, "out-bfs3.tex");
  b.run();
  delete g;
}

int main() {
  // tikz_example();
  // tikz_exampleDFS();
  // tikz_exampleBFS();
  // runtime_iterate();
  runtime_dfs();
}

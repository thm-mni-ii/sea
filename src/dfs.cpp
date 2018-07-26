#include <iostream>
#include "sealib/dfs.h"
#include <stdio.h>
#include <stack>
#include "./segmentstack.h"

using Sealib::DFS;
using Sealib::SegmentStack;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;

static void process_standard(uint u0, Graph *g, uint *color,
                             UserFunc1 preProcess, UserFunc2 preExplore,
                             UserFunc2 postExplore, UserFunc1 postProcess) {
  std::stack<Pair> *s = new std::stack<Pair>;
  s->push(std::make_tuple(u0, 0));
  while (!s->empty()) {
    Pair x = s->top();
    s->pop();
    uint u = std::get<0>(x);
    uint k = std::get<1>(x);
    if (color[u] == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color[u] = DFS_GRAY;
    }
    if (k < g->getNode(u)->getDegree()) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color[v] == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != u0) {
        uint pu = std::get<0>(s->top());
        postExplore(pu, u);
      }
      color[u] = DFS_BLACK;
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
  delete s;
}

static void restore_small(uint, Graph *, CompactArray *, SegmentStack *);

static void process_small(uint u0, Graph *g, CompactArray *color,
                          SegmentStack *s, UserFunc1 preProcess,
                          UserFunc2 preExplore, UserFunc2 postExplore,
                          UserFunc1 postProcess) {
  s->push(Pair(u0, 0));
  Pair x;
  while (!s->empty()) {
    int sr = s->pop(&x);
    if (sr == DFS_NO_MORE_NODES) {
      return;
    }
    uint u, k;
    u = std::get<0>(x);
    k = std::get<1>(x);
    if (color->get(u) == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color->insert(u, DFS_GRAY);
    }
    if (k < g->getNode(u)->getDegree()) {
      s->push(Pair(u, k + 1));
      if (s->isAligned()) return;
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
      if (s->isAligned()) return;
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != u0) {
        Pair px;
        sr = s->pop(&px);
        if (sr == DFS_DO_RESTORE) {
          restore_small(u0, g, color, s);
          s->pop(&px);
        }
        uint pu = std::get<0>(px);
        postExplore(pu, u);
        s->push(px);
      }
      color->insert(u, DFS_BLACK);
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
}

static void restore_small(uint u0, Graph *g, CompactArray *color,
                          SegmentStack *s) {
  s->saveTrailer();
  s->dropAll();
  for (uint a = 0; a < g->getOrder(); a++) {
    if (color->get(a) == DFS_GRAY) {
      color->insert(a, DFS_WHITE);
    }
  }
  process_small(u0, g, color, s, DFS_NOP_PROCESS, DFS_NOP_EXPLORE,
                DFS_NOP_EXPLORE, DFS_NOP_PROCESS);
}

void DFS::standardDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                      UserFunc2 postExplore, UserFunc1 postProcess) {
  uint *color = new uint[g->getOrder()];
  for (uint a = 0; a < g->getOrder(); a++) color[a] = DFS_WHITE;
  for (uint u = 0; u < g->getOrder(); u++) {
    if (color[u] == DFS_WHITE) {
      process_standard(u, g, color, preProcess, preExplore, postExplore,
                       postProcess);
    }
  }
  delete[] color;
}

void DFS::nBitDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                  UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();
  double e = 0.2;
  unsigned q = static_cast<unsigned>(ceil(
      ceil(e / 6 * n) /
      (8 *
       sizeof(Pair))));  // 2q entries on S shall take up at most (e/3)n bits
  unsigned qs = 3;       // stable segment size (?)
  if (q < qs) q = qs;
  unsigned vpg = static_cast<unsigned>(ceil(3 / e));  // 3/e values per group,
                                                      // assume that 3/e is an
                                                      // integer that divides n

  // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
  SegmentStack *s = new SegmentStack(n, q, false);
  CompactArray *color = new CompactArray(n, vpg);
  for (uint a = 0; a < n; a++) color->insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color->get(a) == DFS_WHITE)
      process_small(a, g, color, s, preProcess, preExplore, postExplore,
                    postProcess);
  }
  delete color;
  delete s;
}

class LinearTimeInplaceDFSRunner {
 private:
    unsigned int *A;
    unsigned int n;
    unsigned int N;
    unsigned int startVertex;
    unsigned int startPos;

    // only for debugging
    bool reachedEnd = false;

 public:
    LinearTimeInplaceDFSRunner(unsigned int *graph) : A(graph) {
        this->n = A[0];
        this->N = A[n + 1] + n + 1;
        std::cout << "this->N = " << this->N << std::endl;
    }

    void run(unsigned int startVertex) {
        this->startVertex = startVertex;
        unsigned int p = n + 2;
        std::cout << "Start vertex: " << startVertex << std::endl;
        std::cout << "Start with position: " << p << std::endl;
        while (A[p] != startVertex && p <= N) {
            p = p + 1;
        }

        // TODO(Andrej Sajenko): if p > N => not found => panic
        this->startPos = p;
        visit(p);
    }

 private:
    unsigned int rA(unsigned int i) {
        if (A[i] == 0 || A[i] > n) {
            return A[i];
        } else {
            return A[A[i]];
        }
    }

    void wA(unsigned int i, unsigned int v) {
        if (A[i] == 0 || A[i] > n) {
            A[i] = v;
        } else {
            A[A[i]] = v;
        }
    }

    unsigned int name(unsigned int i) {
        if (A[i] <= n) {
            return A[i];
        } else {
            return 0u;
        }
    }

    void printState() {
        for (int i = 0; i <= this->N; i++) {
            std::cout << this->A[i] << "\t";
            if (i == 0 || i == this->n + 1) {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t   ";
        for (int i = this->n + 2; i <= this->N; i++) {
            std::cout << rA(i) << "\t";
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t   ";
        for (int i = this->n + 2; i <= this->N; i++) {
            std::cout << rA_(i) << "\t";
        }
        std::cout << std::endl;
    }

    unsigned int rA_(unsigned int i) {
        unsigned int Ai = rA(i);
        if (name(i) > 0 || name(Ai) > 0) {
            return Ai;
        } else if (name(Ai - 1) > 0) {
            return rA(Ai - 1);
        } else {
            return rA(Ai);
        }
    }

    void wA_(unsigned int i, unsigned int v) {
        unsigned int Ai = rA(i);
        if (name(i) > 0 || name(Ai) > 0) {
            return wA(i, v);
        } else if (name(Ai - 1) > 0) {
            return wA(Ai - 1, v);
        } else {
            return wA(Ai, v);
        }
    }

    bool isWhite(unsigned int p) {
        bool start = name(p) != this->startVertex;
        unsigned int p1 = A[p + 1]; // where do I point to

        std::cout << "isWhite(" << p << ") == " << start << " && " << name(p1) << " | A[" << 7 << " + 1] = " << p1 <<  std::endl;
        return name(p) != this->startVertex && name(A[p + 1]) != 0;
    }

    void visit(unsigned int p) {
        unsigned int v = name(p);
        if (v == 0) {
            std::cout << "visiting a position that does not store a vertex name" << std::endl;
            exit(1);
        }
        // TODO: if v == 0 => panic

        std::cout << "Visiting vertex: " << v << std::endl; // replace by preprocess
        printState();

        if (reachedEnd) {
            std::cout << "Reached end" << std::endl;
            //return;
        }

        nextNeighbor(p, true);
    };

    void nextNeighbor(unsigned int p, bool ignoreFirstCheck) {
        // First Check
        //std::cout << "enter nextNeighbor(" << p << ")" << std::endl;
        bool namedPos = name(p) != 0;
        if (p > this->N || (namedPos && !ignoreFirstCheck)) {  // Backtracking required
            if (p > this->N) {
                std::cout << "Reached end of memory" << std::endl;
            }

            unsigned int q = p - 1;
            while (name(q) == 0) {
                q = q - 1;
            }
            if (this->startPos == q) {
                std::cout << "end DFS!" << std::endl;
                printState();
                return;
            }
            std::cout << "goToParent(" << q << ") from first check" << std::endl;

            printState();

            if (p > this->N) {
                std::cout << "Reached end of memory" << std::endl;
                this->reachedEnd = true;
            }

            goToParent(q);
        } else {
            unsigned int rap = rA_(p);
            std::cout << "rap(" << rap << ")" << std::endl;
            if (isWhite(rap)) {
                std::cout << "goToChild(" << p << ")" << std::endl;
                goToChild(p);
            } else {
                if (namedPos && !isWhite(rA_(p))) {
                    if (!isWhite(rA_(p + 1))) {
                        nextNeighbor(p + 2, false);
                    } else {
                        unsigned int temp = rA_(p + 1);
                        wA_(p + 1, rA_(p));
                        wA_(p, temp);
                        std::cout << "swap(" << p + 1 << ", " << p << ")" << std::endl;
                        printState();

                        nextNeighbor(p, true);
                    }
                } else {
                    std::cout << "nextNeighbor(" <<  p + 1 << ", false)" << std::endl;
                    nextNeighbor(p + 1, false);
                }
            }
        }
    }

    void goToParent(unsigned int q) { // The root vertex does not get restored very well !!!
        std::cout << "goToParent(" << q << ")" << std::endl;
        unsigned int x = rA_(q + 1);
        unsigned int p = A[q + 1];

        std::cout << "(x = " << x << ", p = " << p << ")" << std::endl;

        wA(q + 1, x + 1); // Restore childs edge => Okay only if we have two vertices.

        if (name(p - 1) == 0) {
            wA(p, q); // Restore the pointer to the child
            nextNeighbor(p + 1, false);
        } else {
            wA(p - 1, q); // Restore the pointer to the child, because we never leave from the second position
            // we left over the first whenever p points to the second.

            if (rA_(p - 1) < rA_(p)) {
                nextNeighbor(p, false);
            } else {
                std::cout << "swap(" << (p - 1) << ", " << p << ")" << std::endl;
                printState();
                unsigned int temp = rA_(p - 1);
                std::cout << "- swap(" << temp << ", " << rA_(p) << ")" << std::endl;
                wA_(p - 1, rA_(p));
                wA_(p, temp);
                printState();
                nextNeighbor(p + 1, false);
            }
        }
    }

    void goToChild(unsigned int p) {
        unsigned int q = rA_(p); // First
        unsigned int v = name(q); // Second

        // Is v discovered from the first position?
        bool fromFirstPosition = name(p) != 0;

        // Third: Create a pointer for later backtrack operation.
        wA(p, rA(q + 1));

        if (fromFirstPosition) {
            wA(q + 1, p + 1);
        } else {
            wA(q + 1, p);
        }

        visit(q);
    };
};

void DFS::runLinearTimeInplaceDFS(unsigned int *graph, unsigned int startVertex) {
    LinearTimeInplaceDFSRunner *ilDFSRunner = new LinearTimeInplaceDFSRunner(graph);
    ilDFSRunner->run(startVertex);
}
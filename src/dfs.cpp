#include <sstream>
#include <iostream>
#include "sealib/dfs.h"
#include <stdio.h>
#include <stack>
#include "./segmentstack.h"

#define GRADE_ZERO 0
#define GRADE_ONE 1
#define GRADE_AT_LEAST_TWO 2

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
    if (k < g->getNodeDegree(u)) {
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
    if (k < g->getNodeDegree(u)) {
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
    unsigned int m_startVertex;
    unsigned int startPos;
    unsigned int qNext = 0;
    unsigned int pBar = 0;

    // only for debugging
    bool reachedEnd = false;

 public:
    LinearTimeInplaceDFSRunner(unsigned int *graph) : A(graph) {
        this->n = A[0];
        this->N = A[n + 1] + n + 1;
        std::cout << "this->N = " << this->N << std::endl;
    }

    void run(const unsigned int t_startVertex) {
        this->m_startVertex = t_startVertex;
        unsigned int p = n + 2;
        std::cout << "Start vertex: " << t_startVertex << std::endl;
        std::cout << "Start with position: " << p << std::endl;
        while (A[p] != t_startVertex && p <= N) {
            p = p + 1;
        }

        // TODO(Andrej Sajenko): if p > N => not found => panic
        this->startPos = p;
        visit(p);
    }

 private:
    unsigned int& As(unsigned int i) {
        if (A[i] == 0 || A[i] > n) {
            return A[i];
        }

        if (A[i] == A[A[i]] || A[A[i]] == 0) {
            return A[i];
        }

        return A[A[i]];
    }

    unsigned int name(unsigned int i) {
        unsigned int x = A[i];

        if (i == x) {
            return x;
        }

        if (x <= n && A[x] != x && A[x] != 0) {
            return A[i];
        }
        return 0u;
    }

    void printState() {
        for (unsigned int i = 0; i <= this->N; i++) {
            std::cout << this->A[i] << "\t";
            if (i == 0 || i == this->n + 1) {
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t   ";
        for (unsigned int i = this->n + 2; i <= this->N; i++) {
            std::cout << As(i) << "\t";
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t   ";
        for (unsigned int i = this->n + 2; i <= this->N; i++) {
            std::cout << rA_(i) << "\t";
        }
        std::cout << std::endl;
    }

    unsigned int rA_(unsigned int i) {
        if (name(i) == 0) {
            return As(i);
        }

        auto p = As(i);
        if (A[p] == p || A[p] == 0) {
            return p;
        }

        if (name(p - 1) != 0) {
            return As(p - 1);
        }

        return p;
    }

    void wA_(unsigned int i, unsigned int v) {
        if (name(i) == 0) {
            As(i) = v;
            return;
        }

        auto p = As(i);
        if (A[p] == p) {
            As(i) = v; // TODO(Andrej) Test
            return;
        }

        if (name(p - 1) != 0) {
            As(p - 1) = v;
            return;
        }

        As(p) = v;
    }

    bool isWhite(unsigned int vertex) {
        auto isVisited = A[A[vertex]] > n || A[vertex] == 0 || (A[vertex] <= n && A[vertex] != vertex);
        auto temp = vertex != this->m_startVertex && !isVisited; // new gray definition
        std::cout << "isWhite(" << vertex << ") = " << temp << std::endl;
        return temp;
        //return name(p) != this->m_startVertex && name(A[p + 1]) != 0;
    }

    int pointsAtNodeOfGrade(unsigned int p) {
        unsigned int x = As(p);
        if (A[x] == x) {
            return GRADE_ZERO;
        }
        // At this point x is a pointer to an adjcency array

        unsigned int ax1 = A[x + 1];
        if (this->n >= A[x] && ax1 <= this->n && ax1 != As(ax1)) {
            return GRADE_ONE;
        }
        return GRADE_AT_LEAST_TWO;
    }

    void visit(unsigned int p) {
        unsigned int v = name(p);
        if (v == 0) {
            std::stringstream ostr;
            ostr << "Position p: " << p << " does not contain a vertex name, it contains the value: " << A[p];
            throw std::invalid_argument(ostr.str());
        }

        std::cout << "Visiting vertex: " << v << std::endl; // replace by preprocess
        printState();

        if (reachedEnd) {
            std::cout << "Reached end" << std::endl;
        }

        std::cout << name(6) << name(7) << name(8) << std::endl;
        std::cout << rA_(6) << rA_(7) << rA_(8) << std::endl;
        std::cout << name(rA_(6)) << name(rA_(7)) << name(rA_(8)) << std::endl;

        nextNeighbor(p, true);
    };

    void swap(unsigned int a, unsigned int b) {
        unsigned int temp = rA_(b);
        wA_(b, rA_(a));
        wA_(a, temp);
    }

    void nextNeighbor(unsigned int p, bool ignoreFirstCheck) {
        std::cout << "enter nextNeighbor(" << p << ")" << std::endl;
        printState();

        // Check if we reached the next adjacency list or the end -> must backtrack
        bool namedPos = name(p) != 0;

        // We ignore this check for visiting the first adjacency entry.
        if (p > this->N || (namedPos && !ignoreFirstCheck)) {  // Backtracking required
            std::cout << "Backtracking required" << std::endl;
            unsigned int q = p - 1;
            while (name(q) == 0) {
                q = q - 1;
            }

            // If we lookup the startposition of the startvertex again the dfs wants to backtrack from the start vertex
            // and we can end the algorithm.
            if (this->startPos == q) {
                std::cout << "end DFS!" << std::endl;
                printState();
                return;
            }

            std::cout << "goToParent(" << q << ") from first check" << std::endl;
            printState();

            if (p > this->N) { // Just for debugging!
                std::cout << "Reached end of memory" << std::endl;
                this->reachedEnd = true;
            }

            return goToParent(q);
        } else {
            // Otherwise p points at a vertex of at least grade 2
            if (name(p) != 0) { // Leaving from first position
                if (isWhite(name(rA_(p)))) {
                    std::cout << "swap(" << p << ", " << (p + 1) << ") = (" << rA_(p) << ", " << rA_(p + 1) << ")" << std::endl;
                    swap(p, p + 1);
                    printState();
                }
                return nextNeighbor(p + 1, false);
            }

            if (name(p - 1) != 0) { // Leaving from second position
                if (isWhite(name(rA_(p)))) {
                    return goToChild(p);
                }
                if (isWhite(name(rA_(p - 1)))) {
                    std::cout << "swap(" << (p - 1) << ", " << p << ")" << std::endl;
                    swap(p - 1, p);
                    printState();
                    return nextNeighbor(p, false);
                }
                return nextNeighbor(p + 1, false);
            }

            // p points at a white vertex and p is not the first or second position
            if (isWhite(name(As(p)))) {
                std::cout << "goToChild(" << p << ")" << std::endl;
                return goToChild(p);
            }

            return nextNeighbor(p + 1, false);
        }
    }

    void goToParent(unsigned int q) {
        std::cout << "goToParent(" << q << ")" << std::endl;

        // If q is of grade 0
        // Will never happen because we can never go into the adjacency array of a node of grade 0, we only peek inside

        // If q is of grade 1
        int grade = pointsAtNodeOfGrade(q);
        switch (grade) {
            case GRADE_ONE: {

            }
            case GRADE_AT_LEAST_TWO: {
                // Otherwise, q is of grade at least two
                unsigned int x = rA_(q); // Value that was stored originaly at q
                unsigned int p = As(q); // Reverse pointer that is stored now at q

                As(q) = x + 1; // Change?
                As(p) = q; // Restore childs edge => Okay only if we have two vertices.

                return nextNeighbor(p + 1, false);
            }
            default: {
                std::stringstream ostr;
                ostr << "Grade type calculation gone wrong, grade type: " << grade << " found, only {0, 1, 2} allowed";
                throw std::invalid_argument(ostr.str());
            }
        }
    }

    // Never call this method with p pointing at a vertex of grade zero!
    void goToChild(unsigned int p) {
        std::cout << "goToChild(" << p << ")" << std::endl;

        unsigned int q = this->qNext == 0 ? rA_(p) : this->qNext;

        int grade = pointsAtNodeOfGrade(p);
        switch (grade) {
            case GRADE_ZERO: {
                std::cout << "next of grade zero" << std::endl;
                // Check if p points at a dead end
                // peek inside, print the node, mark it as visited and continue with the adjacency list

                std::cout << "Visiting vertex: " << q << std::endl; // replace by preprocess
                A[q] = 0; // Mark it as visited with 0.

                printState();

                return nextNeighbor(p, false);
            }
            case GRADE_ONE: {
                std::cout << "next of grade one" << std::endl;
                // Check if p points at a vertex of grade 1
                if (isWhite(name(q))) {
                    if (pBar == 0) {
                        // In this case we are leaving from a node with grade at least two
                        this->pBar = p;
                        this->qNext = q;

                        // Create a reverse pointer and at the same time mark it as visited.
                        As(p) = As(q);
                        As(q) = p;
                        return visit(q);
                    } else {
                        // In this case we are leaving from a node of grade one to a node of grade one.
                        unsigned int u = name(p);
                        this->qNext = As(q); // Next
                        As(q) = u;

                        return visit(q);
                    }
                }

                return nextNeighbor(p + 1, false);
            }
            case GRADE_AT_LEAST_TWO: {
                std::cout << "next of grade at least two" << std::endl;
                if (pBar == 0) {
                    // Create a reverse pointer and at the same time mark it as visited.
                    As(p) = As(q);
                    As(q) = p;

                    return visit(q);
                } else {
                    // We are currently at a one degree vertex and pointing at a vertex of degree at least two
                    auto u = name(p);
                    unsigned int temp = As(q);
                    As(this->pBar) = temp; // TODO
                    As(q) = u;

                    this->qNext = 0;
                    this->pBar = 0;
                }
            }
            default: {
                std::stringstream ostr;
                ostr << "Grade type calculation gone wrong, grade type: " << grade << " found, only {0, 1, 2} allowed";
                throw std::invalid_argument(ostr.str());
            }
        }
    };
};

void DFS::runLinearTimeInplaceDFS(unsigned int *graph, unsigned int startVertex) {
    auto *ilDFSRunner = new LinearTimeInplaceDFSRunner(graph);
    ilDFSRunner->run(startVertex);
}

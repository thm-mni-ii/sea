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
    uint *A;
    uint n;
    uint N;
    uint m_startVertex;
    uint startPos;
    uint pBar = 0;

    UserFunc1 m_preProcess;
    UserFunc1 m_postProcess;

    // only for debugging
    bool reachedEnd = false;

public:
    LinearTimeInplaceDFSRunner(uint *graph, UserFunc1 preProcess, UserFunc1 postProcess)
            : A(graph), n(A[0]), N(n + A[n + 1] + 1),
              m_preProcess(preProcess), m_postProcess(postProcess) { }

    void run(const uint t_startVertex) {
        this->m_startVertex = t_startVertex;
        auto p = n + 2;
        std::cout << "Start vertex: " << t_startVertex << std::endl;
        std::cout << "Start with position: " << p << std::endl;
        while (A[p] != t_startVertex && p <= N) {
            p = p + 1;
        }

        // TODO(Andrej): if p > N => not found => panic

        this->startPos = p;
        visit(p);
        restore();
    }

private:
    void printState() { // Debug Function
        for (uint i = 0; i <= this->N; i++) {
            std::cout << this->A[i] << "\t";
            if (i == 0 || i == this->n + 1) {
                std::cout << " | ";
            }
        }

        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t\t   ";
        for (uint i = this->n + 2; i <= this->N; i++) {
            std::cout << U(i) << "\t";
        }
        std::cout << std::endl;
        std::cout << "\t\t\t\t\t\t\t   ";
        for (uint i = this->n + 2; i <= this->N; i++) {
            std::cout << R(i) << "\t";
        }
        std::cout << std::endl;
    }

    uint &U(uint i) {
        if (name(i) != 0) {
            return A[A[i]];
        }
        return A[i];
    }

    uint name(uint i) {
        if (i == 0 || i == this->n + 1 || i > this->N) {
            std::stringstream ostr;
            ostr << "Never access 0 or n + 1, i = " << i;
            throw std::invalid_argument(ostr.str());
        }

        auto x = A[i];

        if (A[x] == i && x != i) {
            return 0u;
        }

        if (i > this->n && x <= this->n) {
            return x == A[x] ? 0u : x;
        }

        if (i <= this->n) {
            return i;
        }

        if (i == x) {
            return x;
        }

        if (x <= n && A[x] != x && A[x] != 0) {
            return A[i];
        }

        return 0u;
    }

    uint &R(uint i) {
        if (name(i) == 0 || A[i] == A[A[i]]) {
            std::cout << ":";
            return U(i);
        }

        auto p = U(i);
        if (name(p) > 0 /*&& p > this->n || A[p] == p*/) {
            std::cout << ";";
            return U(i);
        }

        // TODO(Andrej): Check that it works for the start vertex.
        if (p <= this->n && A[p] != p) {
            auto next = p;
            while (next <= this->n && A[next] != next) {
                next = U(next);
            }
            std::cout << "|" << p << "|";
            return next;
        }

        return U(p);
    }

    bool isWhite(uint vertex) {
        if (vertex == 0 || vertex > this->n) {
            return false;
        }

        if (vertex == this->m_startVertex) {
            std::cout << "isWhite(" << vertex << ") = false" << std::endl;
            return false;
        }
        auto v = A[vertex];
        if (v == vertex) {
            std::cout << "isWhite(" << vertex << ") = true" << std::endl;
            return true;
        }

        if (A[vertex] <= n && A[A[vertex]] == vertex) {
            std::cout << "isWhite(" << vertex << ") = false" << std::endl;
            return false;
        }
        std::cout << "isWhite(" << vertex << ") = " << (v != 0 && A[v] <= n) << std::endl;
        return v != 0 && A[v] <= n;
    }

    int gradeAtPosition(uint q) {
        if (A[q] == q) {
            return GRADE_ZERO;
        }
        // At this point x is a pointer to an adjacency array
        auto ax1 = A[q + 1];
        if (this->n >= A[q] && ax1 <= this->n && ax1 != U(ax1)) {
            return GRADE_ONE;
        }
        return GRADE_AT_LEAST_TWO;
    }

    int pointsAtNodeOfGrade(uint p) {
        auto x = U(p);
        if (A[x] == x) {
            return GRADE_ZERO;
        }
        // At this point x is a pointer to an adjacency array
        auto ax1 = A[x + 1];
        if (this->n >= A[x] && ax1 <= this->n && ax1 != U(ax1)) {
            return GRADE_ONE;
        }
        return GRADE_AT_LEAST_TWO;
    }

    void visit(uint p) {
        auto v = name(p);
        if (v == 0) {
            std::stringstream ostr;
            ostr << "Position p: " << p << " does not contain a vertex name, it contains the value: " << A[p];
            throw std::invalid_argument(ostr.str());
        }
        this->m_preProcess(v);
        printState();

        if (reachedEnd) {
            std::cout << "Reached end" << std::endl;
        }

        nextNeighbor(p, true);
    }

    void swap(uint a, uint b) {
        auto rA = R(a);
        auto rB = R(b);
        std::cout << "swap(" << a << ", " << b << ")" << "swap_(" << rA << ", " << rB << ")" << std::endl;
        R(b) = rA;
        R(a) = rB;
        printState();
    }

    void nextNeighbor(uint p, bool ignoreCheck) {
        std::cout << "enter nextNeighbor(" << p << ", " << ignoreCheck << ")" << std::endl;
        printState();
        // Check if we reached the next adjacency list or the end -> must backtrack

        // We ignore this check for visiting the first adjacency entry.
        if (p > this->N /* Out of Index */
            || (name(p) != 0 && !ignoreCheck) /* Reached next adjacency array */) {  // Backtracking required
            std::cout << "Backtracking required" << std::endl;
            auto q = p - 1;
            while (name(q) == 0) {
                q = q - 1;
            }

            // If we lookup the startposition of the startvertex again the dfs wants to backtrack from the start vertex
            // and we can end the algorithm.
            if (this->startPos == q) {
                std::cout << "end DFS!" << std::endl;
                this->m_postProcess(this->m_startVertex);
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
            std::cout << "continue with nextNeighbor(" << p << ")" << std::endl;
            // Otherwise p points at a vertex of at least grade 2
            auto p1 = 0u, p2 = 0u;
            auto swapped = false;
            if (name(p) != 0) {
                std::cout << "name(p)" << std::endl;
                p1 = p;
                p2 = p + 1;
                swapped = R(p2) < R(p1);
                std::cout << "check for swapped" << std::endl;
                std::cout << "swapped (" << R(p1) << ", " << R(p2) << ")= " << swapped << std::endl;
            } else if (name(p - 1) != 0) {
                std::cout << "name(p - 1) != 0" << std::endl;
                p1 = p - 1;
                p2 = p;
                swapped = R(p2) < R(p1);
                std::cout << "check for swapped" << std::endl;
                std::cout << "swapped (" << R(p1) << ", " << R(p2) << ")= " << swapped << std::endl;
            }

            if (p == p1) {
                ++p;
                swap(p1, p2);
            } else if (p2 == p && swapped) {
                swap(p1, p2);
            }

            if (isWhite(name(R(p)))) {
                return goToChild(p);
            } else {
                if (p2 == p && R(p2) < R(p1)) {
                    return nextNeighbor(p, false);
                }
                return nextNeighbor(p + 1, false);
            }
        }
    }

    void goToParent(uint q) {
        std::cout << "goToParent(" << q << ")" << std::endl;
        printState();

        // If q is of grade 0
        // Will never happen because we can never go into the adjacency array of a node of grade 0, we only peek inside

        // If q is of grade 1
        int grade = pointsAtNodeOfGrade(q);
        switch (grade) {
            case GRADE_ONE: {
                auto p = A[name(q)];
                std::cout << "back to " << p << std::endl;
                while (p <= this->n) {
                    std::cout << "back to " << p << "from " << q << std::endl;
                    printState();
                    // TODO(Andrej): Postprocess vertices
                    this->m_postProcess(p);
                    std::cout << "endless loop" << std::endl;
                    p = A[name(q)];
                }
                nextNeighbor(p, false);
            }
            case GRADE_AT_LEAST_TWO: {
                this->m_postProcess(name(q));
                // Otherwise, q is of grade at least two
                auto x = R(q); // Value that was stored originaly at q
                auto p = U(q); // Reverse pointer that is stored now at q

                U(q) = x + 1;
                U(p) = q; // Restore childs edge => Okay only if we have two vertices.

                return nextNeighbor(p, false);
            }
            default: {
                std::stringstream ostr;
                ostr << "Grade type calculation gone wrong, grade type: " << grade << " found, only {0, 1, 2} allowed";
                throw std::invalid_argument(ostr.str());
            }
        }
    }

    // Never call this method with p pointing at a vertex of grade zero!
    void goToChild(uint p) {
        std::cout << "goToChild(" << p << ")" << std::endl;

        auto q = R(p);

        int grade = pointsAtNodeOfGrade(p);
        switch (grade) {
            case GRADE_ZERO: {

                // At this pointer q is not a pointer but a vertex name (can be used only in A[1 ... n])
                this->m_preProcess(q);
                this->m_postProcess(q);
                A[q] = p; // Set the reverse pointer for a later restoration.

                printState();
                return nextNeighbor(p, false); // p + 1?
            }
            case GRADE_ONE: {
                std::cout << "next of grade one" << std::endl;

                auto current = p;
                auto next = q;
                while (true) {
                    std::cout << "next: " << next << std::endl;
                    if (isWhite(name(next))) {
                        switch (grade) {
                            case GRADE_ZERO: {
                                std::cout << "about to discover grade zero" << std::endl;
                                // At this pointer q is not a pointer but a vertex name (can be used only in A[1 ... n])
                                this->m_preProcess(next);
                                this->m_postProcess(next);
                                if (name(next) != 0) {
                                    A[next] = name(current); // We reached it from a vertex of degree one.
                                } else {
                                    A[next] = current; // Set the reverse pointer for a later restoration.
                                }


                                // we need to backtrack again.
                                pBar = 0; // Reset pBar to be able to discover the next change from |u| >= 2 to |v| = 1
                                return goToParent(current);
                            }
                            case GRADE_ONE: {
                                std::cout << "about to discover grade one" << std::endl;
                                if (pBar == 0) {
                                    std::cout << "pbar = 0" << std::endl;
                                    // In this case we are leaving from a node with grade at least two
                                    auto v = name(next);
                                    this->m_preProcess(v);

                                    this->pBar = current;

                                    // Create a reverse pointer and at the same time mark it as visited.
                                    auto temp = U(next);
                                    std::cout << "current: " << current << std::endl;
                                    A[v] = current;
                                    std::cout << "after U(next)" << std::endl;
                                    printState();

                                    current = next;
                                    next = temp;
                                } else {
                                    // In this case we are leaving from a node of grade one to a node of grade one.
                                    auto u = name(current);
                                    this->m_preProcess(u);

                                    auto temp = U(next);
                                    U(next) = u; // Set reverse pointer as vertex name
                                    current = next;
                                    next = temp;
                                    // return visit(q); // TODO(Andrej): Visit qNext
                                }
                            }
                                break;
                            case GRADE_AT_LEAST_TWO: {
                                // We are currently at a one degree vertex and pointing at a vertex of degree at least two
                                auto u = name(current);
                                U(this->pBar) = U(next);
                                U(next) = u;

                                // Reset to interrupt handling a one degree vertex chain
                                this->pBar = 0;
                                return visit(next);
                            }
                            default: {
                                std::stringstream ostr;
                                ostr << "Grade type calculation gone wrong, grade type: " << grade
                                     << " found, only {0, 1, 2} allowed";
                                throw std::invalid_argument(ostr.str());
                            }
                        }
                        grade = gradeAtPosition(next);
                    } else {
                        return goToParent(current);
                    }

                    // return visit(next);
                    // return nextNeighbor(p + 1, false);
                }
            }
            case GRADE_AT_LEAST_TWO: {
                std::cout << "next of grade at least two" << std::endl;
                // Create a reverse pointer and at the same time mark it as visited.
                U(p) = U(q);
                U(q) = p;
                return visit(q);
            }
            default: {
                std::stringstream ostr;
                ostr << "Grade type calculation gone wrong, grade type: " << grade << " found, only {0, 1, 2} allowed";
                throw std::invalid_argument(ostr.str());
            }
        }
    };

    void restore() {
//        std::cout << "start restoration" << std::endl;
//        // Solution for degree one only nodes
//        for (uint v = 1; v <= this->n; v++) {
//            if (v != m_startVertex && !isWhite(v)) {
//                A[v] = A[v] - 1;
//            }
//        }
//        std::cout << "restored state" << std::endl;
//        printState();
    }
};

void DFS::runLinearTimeInplaceDFS(uint *graph, UserFunc1 preProcess, UserFunc1 postProcess, uint startVertex) {
    auto *ilDFSRunner = new LinearTimeInplaceDFSRunner(graph, preProcess, postProcess);
    ilDFSRunner->run(startVertex);
    delete ilDFSRunner;
}

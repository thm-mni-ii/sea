#include "sealib/dfs.h"
#include <sstream>
#include <stack>
#include "./segmentstack.h"

using Sealib::DFS;
using Sealib::SegmentStack;
using Sealib::BasicSegmentStack;
using Sealib::ExtendedSegmentStack;
using Sealib::CompactArray;
using Sealib::Graph;
using Sealib::Node;

static void process_standard(uint u0, Graph *g, uint *color,
                             UserFunc1 preProcess, UserFunc2 preExplore,
                             UserFunc2 postExplore, UserFunc1 postProcess) {
  std::stack<Pair> *s = new std::stack<Pair>;
  s->push(Pair(u0, 0));
  while (!s->empty()) {
    Pair x = s->top();
    s->pop();
    uint u = x.head();
    uint k = x.tail();
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
        uint pu = s->top().head();
        postExplore(pu, u);
      }
      color[u] = DFS_BLACK;
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
  delete s;
}

template <class SS>
static void process_small(uint u0, Graph *g, CompactArray *color, SS *s,
                          void (*restoration)(uint, Graph *, CompactArray *,
                                              SS *),
                          UserFunc1 preProcess, UserFunc2 preExplore,
                          UserFunc2 postExplore, UserFunc1 postProcess) {
  s->push(Pair(u0, 0));
  Pair x;
  while (!s->isEmpty()) {
    int sr = s->pop(&x);
    if (sr == DFS_DO_RESTORE) {
      restoration(u0, g, color, s);
      s->pop(&x);
    } else if (sr == DFS_NO_MORE_NODES) {
      return;
    }
    uint u, k;
    u = x.head();
    k = x.tail();
    if (color->get(u) == DFS_WHITE) {
      if (preProcess != DFS_NOP_PROCESS) preProcess(u);
      color->insert(u, DFS_GRAY);
    }
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      uint v = g->head(u, k);
      if (preExplore != DFS_NOP_EXPLORE) preExplore(u, v);
      if (color->get(v) == DFS_WHITE) {
        s->push(Pair(v, 0));
      } else {
        if (postExplore != DFS_NOP_EXPLORE) postExplore(u, v);
      }
    } else {
      if (postExplore != DFS_NOP_EXPLORE && u != u0) {
        Pair px;
        sr = s->pop(&px);
        if (sr == DFS_DO_RESTORE) {
          restoration(u0, g, color, s);
          s->pop(&px);
        }
        uint pu = px.head();
        postExplore(pu, u);
        s->push(px);
      }
      color->insert(u, DFS_BLACK);
      if (postProcess != DFS_NOP_PROCESS) postProcess(u);
    }
  }
}

static void restore_full(uint u0, Graph *g, CompactArray *color,
                         BasicSegmentStack *s) {
  s->saveTrailer();
  s->dropAll();
  for (uint a = 0; a < g->getOrder(); a++) {
    if (color->get(a) == DFS_GRAY) {
      color->insert(a, DFS_WHITE);
    }
  }
  s->push(Pair(u0, 0));
  Pair x;
  while (!s->isAligned()) {
    s->pop(&x);
    uint u = x.head(), k = x.tail();
    if (color->get(u) == DFS_WHITE) color->insert(u, DFS_GRAY);
    if (k < g->getNodeDegree(u)) {
      s->push(Pair(u, k + 1));
      if (s->isAligned()) break;
      uint v = g->head(u, k);
      if (color->get(v) == DFS_WHITE) s->push(Pair(v, 0));
    }
  }
}

/**
 * Get the outgoing edge of u which leads to a gray, top-segment node.
 * @param u node to check
 * @param k starting edge to check
 * @return (c,k) a result tuple with
 *    c: are there more possible nodes to find?
 *    k: outgoing edge that points to a gray, top-segment node (if c is false, k
 * is an 'enclosing' edge)
 */
static std::pair<bool, uint> findEdge(const uint u, const uint k, Graph *g,
                                      CompactArray *c,
                                      ExtendedSegmentStack *s) {
  std::pair<bool, uint> r = std::make_pair(false, static_cast<uint>(-1));
  for (uint i = k; i < g->getNodeDegree(u); i++) {
    uint v = g->head(u, i);
    if (c->get(v) == DFS_GRAY && s->isInTopSegment(v, true)) {
      // std::cout << " found forward edge: " << i << "\n";
      r = std::make_pair(true, i);
      break;
    }
  }
  if (!r.first) {
    Pair a;
    s->getTopTrailer(&a);
    r = std::make_pair(false, a.tail() - 1);
  }
  return r;
}
static void restore_top(uint u0, Graph *g, CompactArray *color,
                        ExtendedSegmentStack *s) {
  Pair x;
  uint u = u0, k = 0;
  if (s->getRestoreTrailer(&x) == 1) {
    std::cout << "starting at bottom " << u << "," << k << "\n";
    color->insert(u, DFS_WHITE);
  } else {
    u = x.head(), k = x.tail() - 1;
    // std::cout << "trailer: " << u << "," << k << "\n";
    u = g->head(u, k), k = s->getOutgoingEdge(u);
    // std::cout << "starting at " << u << "," << k << " \n";
    color->insert(u, DFS_WHITE);
  }
  while (!s->isAligned()) {
    std::pair<bool, uint> r = findEdge(u, k, g, color, s);
    uint u1 = u, k1 = r.second;
    if (r.first) {
      s->push(Pair(u1, k1 + 1));  // k+1 to simulate the normal stack behaviour
      u = g->head(u1, k1);
      k = s->getOutgoingEdge(u);
      color->insert(u, DFS_WHITE);
    } else {
      s->push(Pair(u1, k1 + 1));
      // restoration loop must end now, the stack is aligned
    }
  }
  s->recolorLow(DFS_GRAY);
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

  // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
  BasicSegmentStack s(q);
  CompactArray color(n, 3);
  for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color.get(a) == DFS_WHITE)
      process_small(a, g, &color, &s, restore_full, preProcess, preExplore,
                    postExplore, postProcess);
  }
}

void DFS::nloglognBitDFS(Graph *g, UserFunc1 preProcess, UserFunc2 preExplore,
                         UserFunc2 postExplore, UserFunc1 postProcess) {
  unsigned int n = g->getOrder();

  // printf("e=%3.2f, q=%u, n=%u\n", e, q, n);
  CompactArray color(n, 3);
  ExtendedSegmentStack s(n, g, &color);
  for (uint a = 0; a < n; a++) color.insert(a, DFS_WHITE);
  for (uint a = 0; a < n; a++) {
    if (color.get(a) == DFS_WHITE)
      process_small(a, g, &color, &s, restore_top, preProcess, preExplore,
                    postExplore, postProcess);
  }
}

class LinearTimeInplaceDFSRunner {
 private:
  uint *A;
  uint n;
  uint N;
  uint m_startVertex;
  uint startPos;
  uint pBar = 0;

  uint m_initialStartVertex;
  uint m_initialStartPos;

  UserFunc1 m_preProcess;
  UserFunc1 m_postProcess;

 public:
  LinearTimeInplaceDFSRunner(uint *graph, UserFunc1 preProcess,
                             UserFunc1 postProcess)
      : A(graph),
        n(A[0]),
        N(n + A[n + 1] + 1),
        m_preProcess(preProcess),
        m_postProcess(postProcess) {}

  void run(const uint t_startVertex) {
    this->m_startVertex = t_startVertex;
    this->m_initialStartVertex = t_startVertex;
    auto p = n + 2;
    while (A[p] != t_startVertex && p <= N) {
      p = p + 1;
    }
    this->startPos = p;
    this->m_initialStartPos = p;

    visit(p);

    uint current = m_startVertex;
    while (current != this->m_initialStartVertex) {
      if (isWhite(current)) {
        p = startPos + 1;
        while (A[p] != current && p != m_initialStartPos) {
          ++p;
          if (p > N) {
            p = n + 2;
          }
        }
        this->startPos = p;
        visit(p);
      }
      ++current;
      if (current > this->n) {
        current = 1;
      }
    }

    restore();
  }

 private:
  inline uint &U(uint i) {
    if (name(i) != 0) {
      return A[A[i]];
    }
    return A[i];
  }

  inline uint name(uint i) {
    if (i == 0 || i == this->n + 1 || i > this->N) {
      std::stringstream ostr;
      ostr << "Never access 0 or n + 1, i = " << i << " n: " << n
           << " N: " << N;
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

  inline uint &R(uint i) {
    if (name(i) == 0 || A[i] == A[A[i]]) {
      return U(i);
    }

    auto &p = U(i);
    if (name(p) > 0 /*&& p > this->n || A[p] == p*/) {
      return U(i);
    }

    if (p <= this->n && A[p] != p) {
      auto &next = p;
      while (next <= this->n && A[next] != next) {
        next = U(next);
      }
      return next;
    }

    return U(p);
  }

  inline bool isWhite(uint vertex) {
    if (vertex == 0 || vertex > this->n) {
      return false;
    }

    if (vertex == this->m_startVertex) {
      return false;
    }
    auto v = A[vertex];
    if (v == vertex) {
      return true;
    }

    if (A[vertex] <= n && A[A[vertex]] == vertex) {
      return false;
    }
    return v != 0 && A[v] <= n;
  }

  inline int gradeAtPosition(uint q) {
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

  inline int pointsAtNodeOfGrade(uint p) {
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
      ostr << "Position p: " << p
           << " does not contain a vertex name, it contains the value: "
           << A[p];
      throw std::invalid_argument(ostr.str());
    }
    this->m_preProcess(v);

    nextNeighbor(p, true);
  }

  inline void swap(uint a, uint b) {
    auto rA = R(a);
    auto rB = R(b);
    R(b) = rA;
    R(a) = rB;
  }

  void nextNeighbor(uint p, bool ignoreCheck) {
    // Check if we reached the next adjacency list or the end -> must backtrack

    // We ignore this check for visiting the first adjacency entry.
    if (p > this->N /* Out of Index */
        || (name(p) != 0 && !ignoreCheck) /* Reached next adjacency array */) {
      auto q = p - 1;
      while (name(q) == 0) {
        q = q - 1;
      }

      // If we lookup the startposition of the startvertex again the
      // dfs wants to backtrack from the start vertex
      // and we can end the algorithm.
      if (this->startPos == q) {
        this->m_postProcess(this->m_startVertex);
        return;
      }

      return goToParent(q);
    } else {
      // Otherwise p points at a vertex of at least grade 2
      auto p1 = 0u, p2 = 0u;
      auto swapped = false;
      if (name(p) != 0) {
        p1 = p;
        p2 = p + 1;
        swapped = R(p2) < R(p1);
      } else if (name(p - 1) != 0) {
        p1 = p - 1;
        p2 = p;
        swapped = R(p2) < R(p1);
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
    // If q is of grade 0
    // Will never happen because we can never go into the adjacency
    // array of a node of grade 0, we only peek inside

    // If q is of grade 1
    int grade = pointsAtNodeOfGrade(q);
    switch (grade) {
      case GRADE_ONE: {
        auto p = A[name(q)];
        while (p <= this->n) {
          this->m_postProcess(p);
          p = A[name(q)];
        }
        nextNeighbor(p, false);
      }
      case GRADE_AT_LEAST_TWO: {
        this->m_postProcess(name(q));
        // Otherwise, q is of grade at least two
        auto x = R(q);  // Value that was stored originaly at q
        auto p = U(q);  // Reverse pointer that is stored now at q

        U(q) = x + 1;
        U(p) = q;  // Restore childs edge => Okay only if we have two vertices.

        return nextNeighbor(p, false);
      }
      default: {
        std::stringstream ostr;
        ostr << "Grade type calculation gone wrong, grade type: " << grade
             << " found, only {0, 1, 2} allowed";
        throw std::invalid_argument(ostr.str());
      }
    }
  }

  // Never call this method with p pointing at a vertex of grade zero!
  void goToChild(uint p) {
    auto q = R(p);

    int grade = pointsAtNodeOfGrade(p);
    switch (grade) {
      case GRADE_ZERO: {
        // At this pointer q is not a pointer but a
        // vertex name (can be used only in A[1 ... n])
        this->m_preProcess(q);
        this->m_postProcess(q);
        A[q] = p;  // Set the reverse pointer for a later restoration.

        return nextNeighbor(p, false);
      }
      case GRADE_ONE: {
        auto current = p;
        auto next = q;
        while (true) {
          if (isWhite(name(next))) {
            switch (grade) {
              case GRADE_ZERO: {
                // std::cout << "about to discover grade zero" << std::endl;
                // At this pointer q is not a pointer but a
                // vertex name (can be used only in A[1 ... n])
                this->m_preProcess(next);
                this->m_postProcess(next);
                if (name(next) != 0) {
                  // We reached it from a vertex of degree one.
                  A[next] = name(current);
                } else {
                  // Set the reverse pointer for a later restoration.
                  A[next] = current;
                }

                // we need to backtrack again.
                // Reset pBar to be able to discover the
                // next change from |u| >= 2 to |v| = 1
                pBar = 0;
                return goToParent(current);
              }
              case GRADE_ONE: {
                if (pBar == 0) {
                  // In this case we are leaving from a node
                  // with grade at least two
                  auto v = name(next);
                  this->m_preProcess(v);

                  this->pBar = current;

                  // Create a reverse pointer and at the
                  // same time mark it as visited.
                  auto temp = U(next);
                  A[v] = current;

                  current = next;
                  next = temp;
                } else {
                  // In this case we are leaving from a node of grade one to
                  // a node of grade one.
                  auto u = name(current);
                  this->m_preProcess(u);

                  auto temp = U(next);
                  U(next) = u;  // Set reverse pointer as vertex name
                  current = next;
                  next = temp;
                }
              } break;
              case GRADE_AT_LEAST_TWO: {
                // We are currently at a one degree vertex and pointing
                // at a vertex of degree at least two
                auto u = name(current);
                U(this->pBar) = U(next);
                U(next) = u;

                // Reset to interrupt handling a one degree vertex chain
                this->pBar = 0;
                return visit(next);
              }
              default: {
                std::stringstream ostr;
                ostr << "Grade type calculation gone wrong, grade type: "
                     << grade << " found, only {0, 1, 2} allowed";
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
        // std::cout << "next of grade at least two" << std::endl;
        // Create a reverse pointer and at the same time mark it as visited.
        U(p) = U(q);
        U(q) = p;
        return visit(q);
      }
      default: {
        std::stringstream ostr;
        ostr << "Grade type calculation gone wrong, grade type: " << grade
             << " found, only {0, 1, 2} allowed";
        throw std::invalid_argument(ostr.str());
      }
    }
  }

  inline void restore() {
    // Restoration of vertices of degree at least two
    for (uint v = 1; v <= this->n; v++) {
      if (v != m_startVertex && !isWhite(v)) {
        A[v] = A[v] - 1;
      }
    }

    // Restoration of degree one vertices!
    for (uint p = n + 2; p <= this->N; ++p) {
      auto v = U(p);
      auto u = 0u;
      if (!isWhite(v)) {
        if ((v <= n && name(p) == 0) || ((u = A[v]) && name(u) != 0) ||
            ((u = name(p)) && A[v] == p + 1)) {
          A[v] = v;
        }
      }
    }
  }
};

void DFS::runLinearTimeInplaceDFS(uint *graph, UserFunc1 preProcess,
                                  UserFunc1 postProcess, uint startVertex) {
  auto *ilDFSRunner =
      new LinearTimeInplaceDFSRunner(graph, preProcess, postProcess);
  ilDFSRunner->run(startVertex);
  delete ilDFSRunner;
}

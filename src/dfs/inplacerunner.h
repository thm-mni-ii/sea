#ifndef SRC_DFS_INPLACERUNNER_H_
#define SRC_DFS_INPLACERUNNER_H_

#include <utility>
#include <stdexcept>

#define GRADE_ZERO 0
#define GRADE_ONE 1
#define GRADE_AT_LEAST_TWO 2

namespace Sealib {
/* @author Simon Schniedenharn */
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

  Consumer m_preProcess;
  Consumer m_postProcess;

 public:
  LinearTimeInplaceDFSRunner(uint *graph, Consumer preProcess,
                             Consumer postProcess)
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
        return nextNeighbor(p, false);
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
}  // namespace Sealib
#endif  // SRC_DFS_INPLACERUNNER_H_

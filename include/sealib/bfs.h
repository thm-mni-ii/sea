#ifndef SEALIB_BFS_H_
#define SEALIB_BFS_H_
#include <stdexcept>
#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/graph.h"

#define BFS_WHITE 0
#define BFS_GRAY1 1
#define BFS_GRAY2 2
#define BFS_BLACK 3

typedef void (*UserFunc1)(uint);
typedef void (*UserFunc2)(uint, uint);

#define BFS_NOP_PROCESS (UserFunc1)0
#define BFS_NOP_EXPLORE (UserFunc2)0

namespace Sealib {
class ConnectedComponent {
 private:
  std::vector<Pair> v;

 public:
  ConnectedComponent() {}
  void push(Pair p) { v.push_back(p); }
  Pair get(int p) { return v.at(p); }
  size_t length() { return v.size(); }
  std::vector<Pair> getSequence() { return v; }
};

class BFS {
 public:
  /**
   * Run a breadth-first search over a given graph, while executing the two
   * given user functions.
   * EFFICIENCY: O(n+m) time, O(n) bits
   * @param g the graph to run the BFS over
   * @param preprocess to be executed before processing a node u
   * @param preexplore to be executed before exploring an edge (u,v)
   * @return a vector of CC sequences; each sequence is {(u1,d1),...,(un,dn)}
   * where
   * ui is the node name and di is the distance from ui to the starting
   * node of the connected component.
   */
  static std::vector<ConnectedComponent> run(Graph *g, UserFunc1 preprocess,
                                             UserFunc2 preexplore);
};

}  // namespace Sealib
#endif  // SEALIB_BFS_H_

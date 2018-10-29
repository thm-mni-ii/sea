#include "sealib/reversedfs.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(ReverseDFSTest, postprocess) {
  Graph *g = Sealib::GraphCreator::createRandomFixed(8, 2);
  std::vector<uint> ref;
  DFS::nloglognBitDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                      [&ref](uint u) { ref.push_back(u); });
  std::reverse(ref.begin(), ref.end());
  std::vector<uint> seq;
  ReverseDFS d(g);
  d.init();
  while (d.more()) {
    UserCall a = d.next();
    if (a.type == UserCall::postprocess) {
      seq.push_back(a.u);
    }
  }
  for (uint a : ref) std::cout << a << " ";
  std::cout << "\n---\n";
  for (uint a : seq) std::cout << a << " ";
  EXPECT_EQ(ref.size(), seq.size());
  ASSERT_GT(seq.size(), 0);
  if (seq.size() < ref.size()) {
    std::vector<uint> b;
    for (uint a = 0; a < ref.size(); a++) {
      if (seq[a] != ref[a]) {
        b.emplace_back(ref[a]);
        seq.emplace(seq.begin() + a, ref[a]);
      }
    }
    std::cout << "missing in seq: ";
    for (uint a : b) {
      std::cout << a << " ";
    }
    std::cout << "\n";
  } else {
    for (uint a = 0; a < ref.size(); a++) {
      EXPECT_EQ(seq[a], ref[a]);
    }
  }
}

#include <gtest/gtest.h>
#include <sealib/algorithms/dfs.h>

TEST(DFSTest,push) {
	DFS d=DFS(NULL,NULL,NULL,NULL,NULL);
	Adjacency na=Adjacency(5);
	Node *n=new Node(&na,1);
	d.push(n);
	ASSERT_EQ(d.peek(),n);
}

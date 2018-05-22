#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/algorithms/dfs.h>
#define DFS_DEBUG

TEST(DFSTest,push) {
	Node *nodes=(Node*)malloc(sizeof(Node)*5);
	for(int a=0; a<4; a++) nodes[a]=Node(new Adjacency(-1),0);
	DFS d=DFS(new Graph(nodes,5),NULL,NULL,NULL,NULL);
	d.push(nodes+2);
	ASSERT_EQ(d.peek(),nodes+2);
}

TEST(DFSTest,pop) {
	Node *nodes=(Node*)malloc(sizeof(Node)*5);
	for(int a=0; a<4; a++) nodes[a]=Node(new Adjacency(-1),0);
	DFS d=DFS(new Graph(nodes,5),NULL,NULL,NULL,NULL);
	d.push(nodes+2);
	d.push(nodes+4);
	d.push(nodes+1);
	ASSERT_EQ(d.pop(),nodes+1);
	ASSERT_EQ(d.pop(),nodes+4);
	ASSERT_EQ(d.pop(),nodes+2);
	ASSERT_EQ(d.pop(),(void*)NULL);
}

TEST(DFSTest,process) {
	Node *nodes=(Node*)calloc(1,sizeof(Node)*6); //calloc!
	for(int a=0; a<4; a++) nodes[a]=Node(new Adjacency(a+1),1);
	nodes[5]=Node(new Adjacency(4),1);
	DFS d=DFS(new Graph(nodes,6),NULL,NULL,NULL,NULL);
	d.run();
	ASSERT_EQ(1,1);
	free(nodes);
}

#include <gtest/gtest.h>
#include <stdlib.h>
#include <sealib/dfs.h>
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

TEST(DFSTest,run) {
	int order=6;
	Node *nodes=(Node*)calloc(1,sizeof(Node)*order); //calloc!
	for(int a=0; a<order-2; a++) nodes[a]=Node(new Adjacency(a+1),1);
	nodes[order-1]=Node(new Adjacency(4),1);
	DFS d=DFS(new Graph(nodes,order),NULL,NULL,NULL,NULL);
	for(uint a=0; a<(uint)order; a++) ASSERT_EQ(d.getColor(a),DFS_WHITE);
	d.run();
	for(uint a=0; a<(uint)order; a++) ASSERT_EQ(d.getColor(a),DFS_BLACK);
	free(nodes);
}

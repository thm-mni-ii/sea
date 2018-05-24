#include <gtest/gtest.h>
#include <sealib/stack.h>

TEST(StackTest,push) {
	Node *nodes=(Node*)malloc(sizeof(Node)*5);
	for(int a=0; a<4; a++) nodes[a]=Node(new Adjacency(-1),0);
	Stack d=Stack(3);
	d.push(nodes+2);
	ASSERT_EQ(d.peek(),nodes+2);
	d.push(nodes+1);
	ASSERT_EQ(d.peek(),nodes+1);
	d.push(nodes+0);
	ASSERT_EQ(d.peek(),nodes+0);
	d.push(nodes+1); //this will not be pushed
	ASSERT_EQ(d.peek(),nodes+0);
}

TEST(StackTest,pop) {
	Node *nodes=(Node*)malloc(sizeof(Node)*5);
	for(int a=0; a<4; a++) nodes[a]=Node(new Adjacency(-1),0);
	Stack d=Stack(10);
	d.push(nodes+2);
	d.push(nodes+4);
	d.push(nodes+1);
	ASSERT_EQ(d.pop(),nodes+1);
	ASSERT_EQ(d.pop(),nodes+4);
	ASSERT_EQ(d.pop(),nodes+2);
	ASSERT_EQ(d.pop(),STACK_FAULT);
	ASSERT_EQ(d.peek(),STACK_FAULT);
}

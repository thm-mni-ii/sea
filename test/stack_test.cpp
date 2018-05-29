#include <gtest/gtest.h>
#include <sealib/stack.h>

TEST(StackTest,push) {
	Stack d=Stack(3);
	d.push(2);
	ASSERT_EQ(d.peek(),2);
	d.push(1);
	ASSERT_EQ(d.peek(),1);
	d.push(0);
	ASSERT_EQ(d.peek(),0);
	d.push(1); //this will not be pushed
	ASSERT_EQ(d.peek(),0);
}

TEST(StackTest,pop) {
	Stack d=Stack(10);
	d.push(2);
	d.push(4);
	d.push(1);
	ASSERT_EQ(d.pop(),1);
	ASSERT_EQ(d.pop(),4);
	ASSERT_EQ(d.pop(),2);
	try {
		d.pop();
	} catch(unsigned int e) {
		try {
			d.peek();
		} catch(unsigned int e1) {
			ASSERT_TRUE(true);
			return;
		}
	}
	ASSERT_TRUE(false);
}

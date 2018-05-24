#include "sealib/node.h"
#define STACK_FAULT (Node*)0

class Stack {
	private:
		Node **stack;
		unsigned int sp;
		unsigned int size;
		
	public:
		Stack(unsigned int pSize);
		void push(Node *u);
		Node * pop();
		Node * peek();
};

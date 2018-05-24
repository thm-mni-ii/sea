#include "sealib/node.h"
#define STACK_FAULT (Node*)0

/*
	This stack data structure can hold a given number of Nodes.
	
	(It can be extended to hold multi-valued data by overloading push and pop).
*/
class Stack {
	private:
		Node **stack;
		unsigned int sp;
		unsigned int size;
		
	public:
		/* Create a stack of the given size. */
		Stack(unsigned int pSize);
		
		/* Push a node to the stack if the stack is not full.
			@param u the node to add to the stack
		*/
		void push(Node *u);
		
		/* Pop a node from the stack if the stack is not empty.
			@return if the stack is empty: STACK_FAULT
				else: the node from the top of the stack <br>
		*/
		Node * pop();
		
		/* Read the stack top, without removing it.
			@return if the stack is empty: STACK_FAULT
				else: the top element of the stack
		*/
		Node * peek();
};

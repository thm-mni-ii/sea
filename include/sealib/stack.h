#ifndef STACK_H
#define STACK_H
#include "sealib/node.h"
#define STACK_FAULT (unsigned int)0

/*
	This stack data structure can hold a given number of elements.
*/
class Stack {
	private:
		unsigned int *stack;
		unsigned int sp;
		unsigned int size;
		
	public:
		/* Create a simple node stack of the given size. */
		Stack(unsigned int pSize);
		
		/* Push a node to the stack if the stack is not full.
			@param u the node to add to the stack
		*/
		void push(unsigned int u);
		
		/* Pop a node from the stack if the stack is not empty.
			@return if the stack is empty: STACK_FAULT
				else: the node from the top of the stack <br>
		*/
		unsigned int pop();
		
		/* Read the stack top, without removing it.
			@return if the stack is empty: STACK_FAULT
				else: the top element of the stack
		*/
		unsigned int peek();
};
#endif

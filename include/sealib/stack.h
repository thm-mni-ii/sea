#ifndef STACK_H
#define STACK_H
#include "sealib/node.h"
#define STACK_FAULT (unsigned int)0

/**
 * This stack data structure can hold a given number of elements.
*/
class Stack {
	private:
		unsigned int *stack;
		unsigned int sp;
		unsigned int size;
		
	public:
		/**
		 * Create a simple node stack of the given size.
		 * @param pSize maximum number of elements for this stack
		 */
		Stack(unsigned int pSize);
		~Stack();
		
		/**
		 * Push a node to the stack if the stack is not full.
		 * @param u the node to add to the stack
		 */
		void push(unsigned int u);
		
		/** 
		 * Pop a node from the stack if the stack is not empty.
		 * @return the node from the top of the stack
		 * @throws STACK_FAULT if the stack is empty
		 */
		unsigned int pop();
		
		/* Read the stack top, without removing it.
		 * @return the top element of the stack
		 * @throws STACK_FAULT if the stack is empty
		 */
		unsigned int peek();
};
#endif

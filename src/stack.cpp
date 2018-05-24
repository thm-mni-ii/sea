#include "sealib/stack.h"
#include "sealib/node.h"

void Stack::push(Node *u) { 
	if(sp<size) stack[sp++]=u;
}

Node * Stack::pop() {
	if(sp==0) return STACK_FAULT;
	else return stack[--sp];
}

Node * Stack::peek() {
	if(sp==0) return STACK_FAULT; 
	else return stack[sp-1]; 
}

Stack::Stack(unsigned int pSize) {
	size=pSize;
	stack=new Node*[size];
	sp=0;
}

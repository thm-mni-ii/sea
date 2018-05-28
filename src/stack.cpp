#include "sealib/stack.h"
#include "sealib/node.h"

void Stack::push(unsigned int u) { 
	if(sp<size) stack[sp++]=u;
}

unsigned int Stack::pop() {
	if(sp==0) throw STACK_FAULT;
	else return stack[--sp];
}

unsigned int Stack::peek() {
	if(sp==0) throw STACK_FAULT;
	else return stack[sp-1];
}

Stack::Stack(unsigned int pSize) {
	size=pSize;
	stack=new unsigned int[size];
	sp=0;
}
Stack::~Stack() {
	delete[] stack;
}

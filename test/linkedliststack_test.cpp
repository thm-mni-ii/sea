#include <gtest/gtest.h>
#include <sealib/linkedliststack.h>

TEST(ListListStackTest, linked_list_stack) {
    Sealib::LinkedListStack<int> *stack = new Sealib::LinkedListStack<int>();
    stack->push(5);
    stack->push(4);
    stack->push(3);
    stack->push(2);
    stack->push(1);
    size_t size = stack->size();
    for (int i = 1; i <= size; i++) {
        ASSERT_EQ(stack->peek(), i);
        stack->pop();
    }
    ASSERT_EQ(stack->size(), 0);
    delete stack;
}


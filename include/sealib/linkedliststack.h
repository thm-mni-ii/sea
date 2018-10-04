#ifndef SEALIB_LINKEDLISTSTACK_H_
#define SEALIB_LINKEDLISTSTACK_H_
#include <cstddef>
#include <cassert>
#include <ostream>

namespace Sealib {
template<class T>
class LinkedListStack {
 public:
    struct node {
      T val;
      node *next;
      explicit node(T val_, node *next_ = nullptr) : val(val_), next(next_) {}
    };

    LinkedListStack() : top(nullptr), count(0) {}

    ~LinkedListStack() {
        node *current = top;
        while (current) {
            node *next = current->next;
            delete current;
            current = next;
        }
        top = NULL;
    }

    void push(const T &t) {
        top = new node(t, top);
        count++;
    }

    void push(node *n) {
        n->next = top;
        top = n;
        count++;
    }

    void pop() {
        assert(count > 0);
        node *oldTop = top;

        top = top->next;
        delete oldTop;
        count--;
    }

    T &peek() const {
        assert(count > 0);
        return top->val;
    }

    std::size_t size() {
        return count;
    }

    bool isEmpty() {
        return count == 0;
    }

    friend std::ostream &operator<<(std::ostream &os, const LinkedListStack &stack) {
        node *current = stack.top;
        os << "[";
        while (current) {
            os << current->val;
            node *next = current->next;
            current = next;
            if (current) {
                os << ", ";
            }
        }
        return os << "]";
    }

 private:
    node *top;
    std::size_t count;
};
}  // namespace Sealib
#endif  // SEALIB_LINKEDLISTSTACK_H_

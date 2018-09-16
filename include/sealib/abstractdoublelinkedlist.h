#ifndef SEALIB_DOUBLELINKEDLIST_H_
#define SEALIB_DOUBLELINKEDLIST_H_

namespace Sealib {
class AbstractDoubleLinkedList {
 public:
    AbstractDoubleLinkedList() = default;
    virtual ~AbstractDoubleLinkedList() = default;
    virtual unsigned int get() = 0;
    virtual unsigned int remove(unsigned int idx) = 0;
    virtual bool isEmpty() = 0;
    virtual void clear() = 0;
};
}  // namespace Sealib
#endif  // SEALIB_DOUBLELINKEDLIST_H_

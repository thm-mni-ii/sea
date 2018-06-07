//
// Created by jmeintrup on 06.06.18.
//

#ifndef SEA_DOUBLELINKEDLIST_H
#define SEA_DOUBLELINKEDLIST_H
namespace Sealib {
    class DoubleLinkedList {

    public:
        DoubleLinkedList(void) = default;
        virtual ~DoubleLinkedList(void) = default;
        virtual unsigned int get() = 0;
        virtual unsigned int remove(unsigned int idx) = 0;
        virtual bool isEmpty() = 0;
    };
}
#endif //SEA_DOUBLELINKEDLIST_H

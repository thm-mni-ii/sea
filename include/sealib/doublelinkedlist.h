//
// Created by jmeintrup on 18.05.18.
//

#ifndef SEA_DOUBLELINKEDLIST_H
#define SEA_DOUBLELINKEDLIST_H

#include <vector>
#include <ostream>

namespace Sealib {
    class DoubleLinkedList {
    private:
        std::vector<unsigned int> links;
        unsigned int current;

    public:
        DoubleLinkedList(unsigned int size);
        unsigned int get();
        unsigned int remove(unsigned int idx);
        bool isEmpty();

    };
}
#endif //SEA_DOUBLELINKEDLIST_H

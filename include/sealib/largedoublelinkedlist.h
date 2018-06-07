//
// Created by jmeintrup on 18.05.18.
//

#ifndef SEA_LARGEDOUBLELINKEDLIST_H
#define SEA_LARGEDOUBLELINKEDLIST_H

#include <vector>
#include <ostream>
#include <sealib/doublelinkedlist.h>

namespace Sealib {
    class LargeDoubleLinkedList : public DoubleLinkedList{
    private:
        std::vector<unsigned int> links;
        unsigned int current;

    public:
        ~LargeDoubleLinkedList() override {}
        explicit LargeDoubleLinkedList(unsigned int size);
        unsigned int get() override;
        unsigned int remove(unsigned int idx) override;
        bool isEmpty() override;

    };
}
#endif //SEA_LARGEDOUBLELINKEDLIST_H

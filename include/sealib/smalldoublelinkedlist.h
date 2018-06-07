//
// Created by jmeintrup on 06.06.18.
//

#ifndef SEA_SMALLDOUBLELINKEDLIST_H
#define SEA_SMALLDOUBLELINKEDLIST_H
#include <vector>
#include <ostream>
#include <sealib/doublelinkedlist.h>

namespace Sealib {
    class SmallDoubleLinkedList : public DoubleLinkedList{
    private:
        std::vector<unsigned char> links;
        unsigned char current;

    public:
        explicit SmallDoubleLinkedList(unsigned int size);
        ~SmallDoubleLinkedList() override {};
        unsigned int get() override;
        unsigned int remove(unsigned int idx) override;
        bool isEmpty() override;
    };
}
#endif //SEA_SMALLDOUBLELINKEDLIST_H

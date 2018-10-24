#ifndef SRC_SMALLDOUBLELINKEDLIST_H_
#define SRC_SMALLDOUBLELINKEDLIST_H_

#include <src/doublelinkedlist.h>
#include <vector>
#include <ostream>

namespace Sealib {
class SmallDoubleLinkedList : public DoubleLinkedList {
 private:
    std::vector<unsigned char> links;
    unsigned char current;

 public:
    explicit SmallDoubleLinkedList(unsigned char size);
    ~SmallDoubleLinkedList() override = default;
    unsigned int get() override;
    unsigned int remove(unsigned int idx) override;
    bool isEmpty() override;
    void clear() override;
};
}  // namespace Sealib
#endif  // SRC_SMALLDOUBLELINKEDLIST_H_

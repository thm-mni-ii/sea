#ifndef SRC_LARGEDOUBLELINKEDLIST_H_
#define SRC_LARGEDOUBLELINKEDLIST_H_

#include <src/doublelinkedlist.h>
#include <vector>
#include <ostream>

namespace Sealib {
class LargeDoubleLinkedList : public DoubleLinkedList {
 private:
    std::vector<unsigned int> links;
    unsigned int current;

 public:
    ~LargeDoubleLinkedList() override = default;
    explicit LargeDoubleLinkedList(unsigned int size);
    unsigned int get() override;
    unsigned int remove(unsigned int idx) override;
    bool isEmpty() override;
    void clear() override;
};
}  // namespace Sealib
#endif  //  SRC_LARGEDOUBLELINKEDLIST_H_

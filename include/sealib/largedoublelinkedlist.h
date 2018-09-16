#ifndef SEALIB_LARGEDOUBLELINKEDLIST_H_
#define SEALIB_LARGEDOUBLELINKEDLIST_H_

#include <sealib/abstractdoublelinkedlist.h>
#include <vector>
#include <ostream>

namespace Sealib {
class LargeDoubleLinkedList : public AbstractDoubleLinkedList {
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
#endif  //  SEALIB_LARGEDOUBLELINKEDLIST_H_

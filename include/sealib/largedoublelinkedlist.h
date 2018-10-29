#ifndef SEALIB_LARGEDOUBLELINKEDLIST_H_
#define SEALIB_LARGEDOUBLELINKEDLIST_H_

#include <sealib/doublelinkedlist.h>
#include <vector>
#include <ostream>

namespace Sealib {
/**
 * Implementation of the interface provided by DoubleLinkedList for words of size 32bits.
 */
class LargeDoubleLinkedList : public DoubleLinkedList {
 private:
    std::vector<unsigned int> links;
    unsigned int current;

 public:
    ~LargeDoubleLinkedList() override = default;
    explicit LargeDoubleLinkedList(unsigned int size);
    /**
     * @return the currently pointed to value and removes it, or (unsigned int)-1 if empty.
     */
    unsigned int get() override;

    /**
    * @param element to be removed
    * @return the removed element, or (unsigned int)-1 if the element was not present.
    */
    unsigned int remove(unsigned int idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  //  SEALIB_LARGEDOUBLELINKEDLIST_H_

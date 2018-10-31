#ifndef SEALIB_SMALLDOUBLELINKEDLIST_H_
#define SEALIB_SMALLDOUBLELINKEDLIST_H_

#include <sealib/doublelinkedlist.h>
#include <vector>
#include <ostream>

namespace Sealib {
/**
 * Implementation of the interface provided by DoubleLinkedList for words of size 8bits.
 */
class SmallDoubleLinkedList : public DoubleLinkedList {
 private:
    std::vector<unsigned char> links;
    unsigned char current;

 public:
    explicit SmallDoubleLinkedList(unsigned char size);
    ~SmallDoubleLinkedList() override = default;
    /**
     * @return the currently pointed to value and removes it, or (unsigned char)-1 if empty.
     */
    unsigned int get() override;

     /**
     * @param element to be removed
     * @return the removed element, or (unsigned char)-1 if the element was not present.
     */
    unsigned int remove(unsigned int idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  // SEALIB_SMALLDOUBLELINKEDLIST_H_

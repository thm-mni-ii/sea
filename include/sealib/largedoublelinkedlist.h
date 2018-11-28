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
    std::vector<uint32_t> links;
    uint32_t current;

 public:
    ~LargeDoubleLinkedList() override = default;
    explicit LargeDoubleLinkedList(uint32_t size);
    /**
     * @return the currently pointed to value and removes it, or (uint32_t)-1 if empty.
     */
    uint32_t get() override;

    /**
    * @param element to be removed
    * @return the removed element, or (uint32_t)-1 if the element was not present.
    */
    uint32_t remove(uint32_t idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  //  SEALIB_LARGEDOUBLELINKEDLIST_H_

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
    std::vector<uint8_t> links;
    uint8_t current;

 public:
    explicit SmallDoubleLinkedList(uint8_t size);
    ~SmallDoubleLinkedList() override = default;
    /**
     * @return the currently pointed to value and removes it, or (uint8_t)-1 if empty.
     */
    uint32_t get() override;

     /**
     * @param element to be removed
     * @return the removed element, or (uint8_t)-1 if the element was not present.
     */
    uint32_t remove(uint32_t idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  // SEALIB_SMALLDOUBLELINKEDLIST_H_

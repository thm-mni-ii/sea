#ifndef SRC_COLLECTION_SMALLDOUBLELINKEDLIST_H_
#define SRC_COLLECTION_SMALLDOUBLELINKEDLIST_H_

#include "doublelinkedlist.h"
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
    uint64_t get() override;

     /**
     * @param idx element to be removed
     * @return the removed element, or (uint8_t)-1 if the element was not present.
     */
    uint64_t remove(uint64_t idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  // SRC_COLLECTION_SMALLDOUBLELINKEDLIST_H_

#ifndef SRC_COLLECTION_LARGEDOUBLELINKEDLIST_H_
#define SRC_COLLECTION_LARGEDOUBLELINKEDLIST_H_

#include "doublelinkedlist.h"
#include <vector>
#include <ostream>

namespace Sealib {
/**
 * Implementation of the interface provided by DoubleLinkedList for words of size 32bits.
 */
class LargeDoubleLinkedList : public DoubleLinkedList {
 private:
    std::vector<uint64_t> links;
    uint64_t current;

 public:
    ~LargeDoubleLinkedList() override = default;
    explicit LargeDoubleLinkedList(uint64_t size);
    /**
     * @return the currently pointed to value and removes it, or (uint64_t)-1 if empty.
     */
    uint64_t get() override;

    /**
    * @param element to be removed
    * @return the removed element, or (uint64_t)-1 if the element was not present.
    */
    uint64_t remove(uint64_t idx) override;
    /**
     * @return - true if empty, false otherwise
     */
    bool isEmpty() override;
};
}  // namespace Sealib
#endif  //  SRC_COLLECTION_LARGEDOUBLELINKEDLIST_H_

#ifndef SEALIB_DOUBLELINKEDLIST_H_
#define SEALIB_DOUBLELINKEDLIST_H_
#include <cstdint>

namespace Sealib {
/**
 * Cyclical double linked list interface.
 * Representation of fixed length lists that contain ordered elements (1,2,...,n-1,n).
 * Given the nature of the elements of the list, there is no need to actually store the elements.
 * It is enough to only store the forward and backwards pointers, implemented as integers and not
 * actual pointers. Uses O(2n*w) bits space, all access operations take O(1) time.
 */
class DoubleLinkedList {
 public:
    DoubleLinkedList() = default;
    virtual ~DoubleLinkedList() = default;
    /**
     * @return the currently pointed to value and removes it, or error value if empty.
     */
    virtual uint32_t get() = 0;
    /**
    * @param element to be removed
    * @return the removed element, or error value if the element was not present.
    */
    virtual uint32_t remove(uint32_t idx) = 0;
    /**
     * @return - true if empty, false otherwise
     */
    virtual bool isEmpty() = 0;
};
}  // namespace Sealib
#endif  // SEALIB_DOUBLELINKEDLIST_H_

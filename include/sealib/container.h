#ifndef SEALIB_CONTAINER_H_
#define SEALIB_CONTAINER_H_
#include "sealib/_types.h"

namespace Sealib {
/**
 * An container interface to be used by several classes in this library.
 * @param T the element type that this container holds
 * @author Simon Heuser
 */
template <class T, class I = uint>
class Container {
 public:
    /**
     * Get an element from the container.
     * @param i index
     * @return the element at the given index
     */
    virtual T get(I i) const = 0;

    /**
     * Insert an element into the container.
     * @param i index
     * @param v element to insert
     */
    virtual void insert(I i, T v) = 0;

    virtual ~Container() = default;
};
}  // namespace Sealib
#endif  // SEALIB_CONTAINER_H_

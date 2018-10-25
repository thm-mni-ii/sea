#ifndef SEALIB_TRACKINGALLOCATOR_H_
#define SEALIB_TRACKINGALLOCATOR_H_

#include <sealib/bytecounter.h>
#include <cstdlib>
#include <memory>

namespace Sealib {
/**
 * Implementation of the allocator interface with the added functionality to track the
 * space that was allocated in bytes. The Counter used to store this information is accesses via the
 * ByteCounter class. All functions are the same as the regular std::allocator, except that
 * we update the ByteCounter during allocate and deallocate.
 */
template<class T>
class TrackingAllocator {
 public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using void_pointer = void *;
    using const_void_pointer = const void *;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    TrackingAllocator() = default;
    ~TrackingAllocator() = default;

    pointer allocate(size_type numObjects) {
        ByteCounter::get() += sizeof(T) * numObjects;
        return static_cast<pointer>(operator new(sizeof(T) * numObjects));
    }

    pointer allocate(size_type numObjects, const_void_pointer hint) {
        return allocate(numObjects);
    }

    void deallocate(pointer p, size_type numObjects) {
        ByteCounter::get() -= sizeof(T) * numObjects;
        operator delete(p);
    }

    template<class U, class... Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U *p) {
        p->~U();
    }

    template<class U>
    size_type count() {
        return ByteCounter::get();
    }
};
}  // namespace Sealib
#endif  // SEALIB_TRACKINGALLOCATOR_H_

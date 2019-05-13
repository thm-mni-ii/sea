#ifndef SEALIB_TRACKINGALLOCATOR_H_
#define SEALIB_TRACKINGALLOCATOR_H_

#include <sealib/bytecounter.h>
#include <cstdlib>
#include <memory>
#include <limits>

namespace Sealib {
/**
 * Implementation of the allocator interface with the added functionality to track the
 * space that was allocated in bytes. The Counter used to store this information is accesses via the
 * ByteCounter class. All functions are the same as the regular std::allocator,
 * except that we update the ByteCounter during allocate and deallocate.
 */
template<class T>
class TrackingAllocator {
 public:
    /**
     * These definitions are needed for the allocator interface.
     */
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using void_pointer = void *;
    using const_void_pointer = const void *;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    /**
     * Needed to rebind a TrackingAllocator<U> to TrackingAllocator<U'>,
     * if needed by the container using TrackingAllocator.
     * @tparam U element type
     */
    template<class U>
    struct rebind {
      using other = TrackingAllocator<U>;
    };

    TrackingAllocator() = default;

    /**
     * Needed to rebind a TrackingAllocator<U> to TrackingAllocator<U'>,
     * if needed by the container using TrackingAllocator.
     * @tparam U element type
     */
    template<class U>
    TrackingAllocator(const TrackingAllocator<U> &) {}

    /**
     * Allocates numObjects *sizeof(T) * bytes space and writes that
     * number into a counter that tracks all allocated memory.
     * Otherwise works exactly as std::allocator<T>
     */
    pointer allocate(size_type numObjects) {
        ByteCounter::get() += sizeof(T) * numObjects;
        return static_cast<pointer>(operator new(sizeof(T) * numObjects));
    }

    /**
     * Allocates numObjects *sizeof(T) * bytes space and writes that
     * number into a counter that tracks all allocated memory.
     * Otherwise works exactly as std::allocator<T>
     * The param hint is generally used as a pointer to some
     * free memory that might be able to hold the requested memory.
     * Used to speed up allocation if a custom allocator implements it.
     * We do not implement it here, but instead just want the regular allocation
     * function to be called. This is rarely needed.
     */
    pointer allocate(size_type numObjects, const_void_pointer hint) {
        (void) hint;
        return allocate(numObjects);
    }

    /**
     * Deallocates memory that was previously allocated at pointer p of the size
     * of numObjects * sizeof(T). Substracts the now freed memory from the global
     * counter ByteCounter.
     * Otherwise works exactly as std::allocator::deallocate
     */
    void deallocate(pointer p, size_type numObjects) {
        ByteCounter::get() -= sizeof(T) * numObjects;
        operator delete(p);
    }

    /**
     * Maximum size possibly for allocation, depends on hardware used.
     */
    size_type max_size() const {
        return std::numeric_limits<size_type>::max();
    }

    /**
     * Construct is used for the construction of the objects after allocation.
     * Same as std::allocator
     */
    template<class U, class... Args>
    void construct(U *p, Args &&...args) {
        new(p) U(std::forward<Args>(args)...);
    }

    /**
     * Destroy is used on the objects before deallocation.
     * Same as std::allocator
     */
    template<class U>
    void destroy(U *p) {
        p->~U();
    }

    /**
     * @return - currently allocated bytes by all TrackingAllocator classes.
     *
     * Important: Even though TrackingAllocator is a templated class,
     * all versions of the TrackingAllocator share the same ByteCounter!
     */
    template<class U>
    size_type count() {
        return ByteCounter::get();
    }
};
}  // namespace Sealib
#endif  // SEALIB_TRACKINGALLOCATOR_H_

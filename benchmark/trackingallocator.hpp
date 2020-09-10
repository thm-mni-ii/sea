#include <cstddef>
#include <memory>

struct AllocationMetrics
{
    size_t total_allocated = 0;
    size_t total_deallocated = 0;

    void* allocate(size_t s) {
        auto * ret = (size_t*) malloc(sizeof(size_t) + s);
        *ret = s;
        total_allocated+=s;
        return (void*) &ret[1];
    }

    void deallocate(void* ptr) {
        auto * s_ptr = (size_t*) ptr;
        s_ptr--;
        size_t s = *s_ptr;
        total_deallocated+=s;
        free((void*) s_ptr);
    }

    size_t current() {
        return total_allocated - total_deallocated;
    }
};

static AllocationMetrics s_alloc_metrics {};

void* operator new(size_t size) {
    return s_alloc_metrics.allocate(size);
}
void operator delete  (void* ptr) noexcept {
    s_alloc_metrics.deallocate(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept {
    s_alloc_metrics.deallocate(ptr);
}

size_t get_total_allocated_bytes() {
    return s_alloc_metrics.total_allocated;
}

size_t get_total_deallocated_bytes() {
    return s_alloc_metrics.total_allocated;
}

size_t get_current_allocated_bytes() {
    return s_alloc_metrics.current();
}
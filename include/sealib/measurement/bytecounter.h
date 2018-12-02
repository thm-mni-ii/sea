#ifndef SEALIB_BYTECOUNTER_H_
#define SEALIB_BYTECOUNTER_H_
#include <cstdlib>

namespace Sealib {
/**
 * Used as a static container for the TrackingAllocator.
 * This is needed because TrackingAllocator is a template class.
 * Otherwise each templated version would use it's own static variable.
 */
class ByteCounter {
 public:
    /**
     * @return reference to static counter
     */
    static size_t &get();
};
}  // namespace Sealib
#endif  // SEALIB_BYTECOUNTER_H_

#ifndef SEALIB_ITERATOR_ITERATOR_H_
#define SEALIB_ITERATOR_ITERATOR_H_
#include <functional>

namespace Sealib {
/**
 * An iterator interface to be used by several classes in this library.
 * @param T the element type that this iterator iterates over
 * @author Simon Heuser
 */
template <class T>
class Iterator {
 protected:
    /**
     * Initialize the iterator.
     */
    virtual void init() = 0;

    /**
     * Check if the iterator has another element that can be retrieved.
     * @return true if there is another element, false otherwise
     */
    virtual bool more() = 0;

    /**
     * Get the next element from this iterator.
     * @return the next element
     * @throws std::logic_error if there are no more elements
     */
    virtual T next() = 0;

    /**
     * Step through the entire range of the iterator and execute a given function for each encountered element. 
     * (init() before calling this method!)
     * @param f function to execute for each element
     */
    virtual void forEach(std::function<void(T)> f) {
        while (more()) {
            f(next());
        }
    }

    virtual ~Iterator() = default;
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_ITERATOR_H_

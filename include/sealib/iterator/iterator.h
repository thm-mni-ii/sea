#ifndef SEALIB_ITERATOR_ITERATOR_H_
#define SEALIB_ITERATOR_ITERATOR_H_
#include <functional>

namespace Sealib {
/**
* Representation of a user call. A sequence of these can be retrieved from
* the ReverseDFS iterator.
*/
struct UserCall {
    enum Type { nop = 0, preprocess, preexplore, postexplore, postprocess };

    /**
     * Create a UserCall object of the given type.
     * @param t Type of the user call
     * @param p1 first number
     * @param p2 second number (optional, depending on user call type)
     */
    UserCall(uint8_t t, uint64_t p1, uint64_t p2 = 0) : type(t), u(p1), k(p2) {}

    constexpr UserCall() : type(nop), u(0), k(0) {}

    bool operator==(UserCall p) const {
        return type == p.type && u == p.u && k == p.k;
    }
    bool operator!=(UserCall p) const { return !(*this == p); }

    uint8_t type;
    uint64_t u, k;
};
/**
 * An iterator interface to be used by several classes in this library.
 * @tparam T the element type that this iterator iterates over
 * @author Simon Heuser
 */
template <class T>
class Iterator {
  public:
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
     * Step through the entire range of the iterator and execute a given
     * function for each encountered element. (init() before calling this
     * method!)
     * @param f function to execute for each element
     */
    virtual void forEach(std::function<void(T)> f) {
        while (more()) {
            f(next());
        }
    }

  protected:
    virtual ~Iterator() = default;
    Iterator() = default;
    Iterator(Iterator const &) = default;
    Iterator(Iterator &&) = default;
    Iterator &operator=(Iterator const &) = default;
    Iterator &operator=(Iterator &&) = default;
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_ITERATOR_H_

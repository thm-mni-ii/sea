#ifndef SEALIB_ITERATOR_H_
#define SEALIB_ITERATOR_H_

#include "sealib/choicedictionary.h"

namespace Sealib {

class Iterator {
#define SHIFT_OFFSET 1UL

   private:
    unsigned long int activeBlock, blockValue;
    ChoiceDictionary *choicedictionary;

   public:
    /**
     *
     */
    explicit Iterator(ChoiceDictionary *_choicedictionary);

    /**
     *
     */
    void init();

    /**
     *
     */
    bool more();

    /**
     *
     */
    unsigned long int next();

    /**
     *
     */
    // void close();

    ~Iterator();
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_H_

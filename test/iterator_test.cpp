#include <gtest/gtest.h>
#include <sealib/iterator.h>

using Sealib::ChoiceDictionary;

TEST(IteratorTest, iterator_integrity) {
    unsigned long int size = 4096UL;
    ChoiceDictionary c = ChoiceDictionary(size);

    // c.insert(50UL);
    // c.insert(55UL);
    // c.insert(60UL);
    c.insert(65UL);
    c.insert(70UL);
    c.insert(150UL);
    c.insert(210UL);
    c.insert(240UL);
    c.insert(621UL);
    c.insert(4000UL);

}

#include <gtest/gtest.h>
#include <sealib/iterator/choicedictionaryiterator.h>

#include <algorithm>
#include <array>
#include <random>

using Sealib::ChoiceDictionaryIterator;

TEST(ChoiceDictionaryIteratorTest, iterator_integrity) {
    const uint64_t size = 25000;
    const uint64_t setSize = 5000;

    Sealib::ChoiceDictionary *c = new Sealib::ChoiceDictionary(size);
    ChoiceDictionaryIterator *iterator = new ChoiceDictionaryIterator(c);

    std::array<uint64_t, setSize> set;

    uint32_t count = 0;
    uint64_t num = 0;
    uint32_t seed = 0;

    for (uint64_t i = 0; i < setSize; i++) {
        set[i] = num;
        num += 5;
    }

    std::shuffle(set.begin(), set.end(), std::default_random_engine(seed));

    for (uint64_t number : set) {
        c->insert(number);
    }
    for (uint64_t i = 0; i < 64; i+=5) {
        c->remove(i);
    }
    c->remove(12000);
    c->remove(12005);
    c->remove(15000);
    c->remove(16000);
    c->remove(16500);

    iterator->init();
    while (iterator->more()) {
        uint64_t index = iterator->next();
        for (uint64_t number : set) {
            if (index == number) {
                count++;
                break;
            }
        }
    }
    // ASSERT_EQ(count, setSize - 18);
    delete iterator;
    delete c;
}

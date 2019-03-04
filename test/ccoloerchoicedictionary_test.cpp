#include <gtest/gtest.h>
#include <sealib/ccolorchoicedictionary.h>

#include <array>
#include <random>


using Sealib::CcolorChoiceDictionary;

TEST(CColorChoiceDictionaryTest, ccolorchoicedictionary_integrity) {
    const unsigned long int size = 1000;
    CcolorChoiceDictionary *cd = new CcolorChoiceDictionary(size, 4);

    cd->insert(255UL, 3);
    cd->insert(256UL, 3);
    cd->insert(0UL, 3);
    cd->insert(31UL, 3);
    cd->insert(32UL, 3);
    cd->insert(127UL, 3);
    cd->insert(128UL, 3);
    cd->insert(129UL, 3);
    cd->insert(257UL, 3);
    /*
    cd->insert(0UL, 3);
    cd->insert(31UL, 3);
    cd->insert(32UL, 3);
    cd->insert(127UL, 3);
    cd->insert(128UL, 3);
    cd->insert(129UL, 3);
    cd->insert(255UL, 3);
    cd->insert(256UL, 3);
    */
    cd->insert(512UL, 2);
    cd->insert(513UL, 2);
    /*
    cd->insert(13UL, 1);
    cd->insert(14UL, 2);

    cd->insert(999UL, 2);
    */
    delete cd;
}

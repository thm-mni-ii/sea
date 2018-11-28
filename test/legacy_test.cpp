#include "sealib/legacy.h"
#include <gtest/gtest.h>
#include <stdio.h>

namespace Sealib {

#ifdef __clang__
/* G++ will complain about this (the tests should be successful anyway): */
extern "C" {
#endif

#define cset Sealib_ChoiceDictionary_set
#define cget Sealib_ChoiceDictionary_get
#define choice Sealib_ChoiceDictionary_choice
#define init Sealib_ChoiceDictionaryIterator_init
#define more Sealib_ChoiceDictionaryIterator_more
#define next Sealib_ChoiceDictionaryIterator_next
#define bset Sealib_Bitset_set
#define bget Sealib_Bitset_get
#define rank Sealib_RankSelect_rank
#define select Sealib_RankSelect_select

TEST(LegacyTest, choiceDictionaryAndIterator) {
    void *cd = Sealib_ChoiceDictionary_new(100);
    cset(cd, 14);
    cset(cd, 32);
    cset(cd, 20);
    ASSERT_TRUE(choice(cd) == 14 || choice(cd) == 20 || choice(cd) == 32);
    void *i = Sealib_ChoiceDictionaryIterator_new(cd);
    init(i);
    EXPECT_EQ(next(i), 14);
    EXPECT_EQ(next(i), 20);
    EXPECT_EQ(next(i), 32);
    EXPECT_FALSE(more(i));
    ASSERT_NO_FATAL_FAILURE(Sealib_ChoiceDictionaryIterator_delete(i));
    ASSERT_NO_FATAL_FAILURE(Sealib_ChoiceDictionary_delete(cd));
}

TEST(LegacyTest, bitsetAndRankSelect) {
    void *b = Sealib_Bitset_new(100);
    bset(b, 40);
    bset(b, 21);
    bset(b, 65);
    ASSERT_TRUE(bget(b, 21) & bget(b, 40) & bget(b, 65));
    void *rs = Sealib_RankSelect_new(b);
    EXPECT_EQ(rank(rs, 21), 0);
    EXPECT_EQ(rank(rs, 22), 1);
    EXPECT_EQ(select(rs, 2), 41);
    EXPECT_EQ(select(rs, 4), (uint64_t)-1);
    ASSERT_NO_FATAL_FAILURE(Sealib_RankSelect_delete(rs));
    ASSERT_NO_FATAL_FAILURE(Sealib_Bitset_delete(b));
}

static uint64_t c = 0;
static void count(uint32_t u) { c += u; }

TEST(LegacyTest, dfs) {
    void *g = Sealib_Graph_generateRandom(101);
    Sealib_DFS_nloglognBitDFS(g, count, NULL, NULL, NULL);
    EXPECT_EQ(c, 5050);
}

#ifdef __clang__
}
#endif

}  // namespace Sealib

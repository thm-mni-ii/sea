#include <gtest/gtest.h>
#include "sealib/matchedint.h"

TEST(MatchedIntTest, constructor) {
    MatchedInt *i = new MatchedInt(1);

    ASSERT_EQ(i->get_value(), 1);
    ASSERT_EQ(i->get_match(), nullptr);
}

TEST(MatchedIntTest, getvalue) {
    int imin = std::numeric_limits<int>::min(); // minimum value
    int imax = std::numeric_limits<int>::max();

    MatchedInt *i = new MatchedInt(0);
    ASSERT_EQ(i->get_value(),0);

    i = new MatchedInt(1231230);
    ASSERT_EQ(i->get_value(),1231230);

    i = new MatchedInt(imin);
    ASSERT_EQ(i->get_value(), imin);

    i = new MatchedInt(imax);
    ASSERT_EQ(i->get_value(), imax);
}

TEST(MatchedIntTest, match) {
    MatchedInt *i = new MatchedInt(1);
    MatchedInt *j = new MatchedInt(2);

    ASSERT_EQ(i->get_match() == nullptr, j->get_match() == nullptr);
    ASSERT_EQ(i->has_match(), false);
    ASSERT_EQ(j->has_match(), false);

    i->match(j);
    ASSERT_EQ(i, j->get_match());
    ASSERT_EQ(j, i->get_match());
    ASSERT_EQ(i->get_value(), 1);
    ASSERT_EQ(j->get_value(), 2);
    ASSERT_EQ(i->has_match(), true);
    ASSERT_EQ(j->has_match(), true);
    i->unmatch();
    ASSERT_EQ(i->get_match() == nullptr, j->get_match() == nullptr);
    ASSERT_EQ(i->has_match(), false);
    ASSERT_EQ(j->has_match(), false);

    i = new MatchedInt(12);
    j = new MatchedInt(500);
    j->match(i);
    ASSERT_EQ(i, j->get_match());
    ASSERT_EQ(j, i->get_match());
    ASSERT_EQ(i->get_value(), 12);
    ASSERT_EQ(j->get_value(), 500);
    ASSERT_EQ(i->has_match(), true);
    ASSERT_EQ(j->has_match(), true);
    j->unmatch();
    ASSERT_EQ(i->get_match() == nullptr, j->get_match() == nullptr);
    ASSERT_EQ(i->has_match(), false);
    ASSERT_EQ(j->has_match(), false);
}




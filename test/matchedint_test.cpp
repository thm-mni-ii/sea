#include <gtest/gtest.h>
#include "sealib/crosslinkedint.h"

TEST(MatchedIntTest, constructor) {
    CrossLinkedInt *i = new CrossLinkedInt(1);

    ASSERT_EQ(i->get_value(), 1);
    ASSERT_EQ(i->get_match(), nullptr);
}

TEST(MatchedIntTest, getvalue) {
    unsigned int imin = std::numeric_limits<unsigned int>::min(); // minimum value
    unsigned int imax = std::numeric_limits<unsigned int>::max();

    CrossLinkedInt *i = new CrossLinkedInt(0);
    ASSERT_EQ(i->get_value(),0);

    i = new CrossLinkedInt(1231230);
    ASSERT_EQ(i->get_value(),1231230);

    i = new CrossLinkedInt(imin);
    ASSERT_EQ(i->get_value(), imin);

    i = new CrossLinkedInt(imax);
    ASSERT_EQ(i->get_value(), imax);
}

TEST(MatchedIntTest, match) {
    CrossLinkedInt *i = new CrossLinkedInt(1);
    CrossLinkedInt *j = new CrossLinkedInt(2);

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

    i = new CrossLinkedInt(12);
    j = new CrossLinkedInt(500);
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




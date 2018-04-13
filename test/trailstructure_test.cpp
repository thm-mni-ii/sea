//
// Created by jmeintrup on 11.03.18.
//

#include <gtest/gtest.h>
#include <sealib/trailstructure.h>

TEST(TrailStructureTest, enter) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.enter(2), 3);
    ASSERT_EQ(ts.enter(4), 0);
    ASSERT_EQ(ts.enter(1), (unsigned int) -1); //last element
}

TEST(TrailStructureTest, leave) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.leave(), 0);

    ts = TrailStructure(5);

    ASSERT_EQ(ts.leave(), 0);
    ASSERT_EQ(ts.leave(), 1);
    ASSERT_EQ(ts.leave(), 2);
    ASSERT_EQ(ts.leave(), 3);
    ASSERT_EQ(ts.leave(), 4);
}

TEST(TrailStructureTest, matches) {
    TrailStructure ts = TrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ASSERT_EQ(ts.getMatched(0), 4);
    ASSERT_EQ(ts.getMatched(1), 1); //has no match, same idx returned
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
    ASSERT_EQ(ts.getMatched(4), 0);
}

TEST(TrailStructureTest, marry) {
    TrailStructure ts = TrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ts.marry(0,1);

    ASSERT_EQ(ts.getMatched(0), 1);
    ASSERT_EQ(ts.getMatched(1), 0); //had no match before
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
    ASSERT_EQ(ts.getMatched(4), 4); //unmatched now
}

TEST(TrailStructureTest, enterLeaveCombination) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.enter(0), 1);
    ASSERT_EQ(ts.enter(4), 2);
    ASSERT_EQ(ts.leave(), 3);
}
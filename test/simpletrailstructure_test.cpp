#include <gtest/gtest.h>
#include "../src/trail/simpletrailstructure.h"
#include "../src/dyck/dyckwordlexicon.h"
#include "../src/dyck/dyckmatchingstructure.h"
#include "../src/trail/trailstructure.h"

using Sealib::SimpleTrailStructure;
using Sealib::INVALID;

TEST(SimpleTrailStructureTest, enter) {
    SimpleTrailStructure ts = SimpleTrailStructure(5);

    ASSERT_EQ(ts.enter(2), 3);
    ASSERT_EQ(ts.enter(4), 0);
    ASSERT_EQ(ts.enter(1), INVALID);  // last element
}

TEST(SimpleTrailStructureTest, leave) {
    SimpleTrailStructure ts = SimpleTrailStructure(5);

    ASSERT_EQ(ts.leave(), 0);

    ts = SimpleTrailStructure(5);

    ASSERT_EQ(ts.leave(), 0);
    ASSERT_EQ(ts.leave(), 1);
    ASSERT_EQ(ts.leave(), 2);
    ASSERT_EQ(ts.leave(), 3);
    ASSERT_EQ(ts.leave(), 4);
}

TEST(SimpleTrailStructureTest, matches) {
    SimpleTrailStructure ts = SimpleTrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ASSERT_EQ(ts.getMatched(0), 4);
    ASSERT_EQ(ts.getMatched(1), 1);  // has no match, same idx returned
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
    ASSERT_EQ(ts.getMatched(4), 0);
}

TEST(SimpleTrailStructureTest, marry) {
    SimpleTrailStructure ts = SimpleTrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ts.marry(0, 1);

    ASSERT_EQ(ts.getMatched(0), 1);
    ASSERT_EQ(ts.getMatched(1), 0);  // had no match before
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
    ASSERT_EQ(ts.getMatched(4), 4);  // unmatched now
}

TEST(SimpleTrailStructureTest, enterLeaveCombination) {
    SimpleTrailStructure ts = SimpleTrailStructure(5);

    ASSERT_EQ(ts.enter(0), 1);
    ASSERT_EQ(ts.enter(4), 2);
    ASSERT_EQ(ts.leave(), 3);
}

#include <gtest/gtest.h>
#include <sealib/trailstructure.h>
#include <sealib/dyckwordlexicon.h>
#include <sealib/simpletrailstructure.h>

using Sealib::TrailStructure;

TEST(TrailStructureTest, enter) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.enter(2), 3);
    ASSERT_EQ(ts.enter(4), 0);
    ASSERT_EQ(ts.enter(1), (unsigned int) -1);  // last element
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
    ASSERT_EQ(ts.getMatched(1), 1);  // has no match, same idx returned
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
    ASSERT_EQ(ts.getMatched(4), 0);
}

TEST(TrailStructureTest, marry) {
    TrailStructure ts = TrailStructure(5);

    ts.enter(2);
    ts.enter(4);
    ts.enter(1);

    ASSERT_EQ(ts.getMatched(0), 4);
    ASSERT_EQ(ts.getMatched(4), 0);
    ASSERT_EQ(ts.getMatched(1), 1);
    ts.marry(0, 1);
    ASSERT_EQ(ts.getMatched(0), 1);
    ASSERT_EQ(ts.getMatched(1), 0);
    ASSERT_EQ(ts.getMatched(4), 4);

    ASSERT_EQ(ts.getMatched(0), 1);
    ASSERT_EQ(ts.getMatched(1), 0);  // had no match before
    ASSERT_EQ(ts.getMatched(2), 3);
    ASSERT_EQ(ts.getMatched(3), 2);
}

TEST(TrailStructureTest, enterLeaveCombination) {
    TrailStructure ts = TrailStructure(5);

    ASSERT_EQ(ts.enter(0), 1);
    ASSERT_EQ(ts.enter(4), 2);
    ASSERT_EQ(ts.leave(), 3);
}

TEST(SimpleTrailStructureTest, allEvenPossibilities) {
    const unsigned long maxLen = 16;
    for (unsigned len = 2; len < maxLen; len += 2) {
        Sealib::DyckWordLexicon lex(len);
        for (const Sealib::Bitset<unsigned char> &word : lex.getLexicon()) {
            std::vector<std::vector<unsigned long>> depths(len);
            for (unsigned int j = 0; j < len; j++) {
                if (word[j]) {
                    unsigned long match =
                        Sealib::DyckMatchingStructure<>::getMatchNaive(word, j);
                    unsigned long d = match - j;
                    depths[d].push_back(j);
                }
            }

            for (unsigned int k = 0; k < len; k++) {
                std::vector<std::vector<unsigned long>> shiftedDepths(depths);
                for (auto &shiftedDepth : shiftedDepths) {
                    for (unsigned long &j : shiftedDepth) {
                        j = (j + k) % len;
                    }
                }
                Sealib::SimpleTrailStructure simpleTrailStructure(len);
                Sealib::TrailStructure trailStructure(len);

                for (std::vector<unsigned long> &depthVector : shiftedDepths) {
                    if (!depthVector.empty()) {
                        for (unsigned long &idx : depthVector) {
                            simpleTrailStructure.enter(static_cast<unsigned int>(idx));
                            trailStructure.enter(static_cast<unsigned int>(idx));
                        }
                    }
                }
                for (unsigned int i = 0; i < len; i++) {
                    unsigned int simpleMatch = simpleTrailStructure.getMatched(i);
                    unsigned int match = trailStructure.getMatched(i);
                    ASSERT_NE(simpleMatch, i);
                    ASSERT_NE(match, i);
                    ASSERT_EQ(simpleMatch, match);
                }
            }
        }
    }
}

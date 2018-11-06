#include <sealib/recursivedyckmatchingstructure.h>
#include <cmath>
#include <iostream>

template <typename BlockType>
Sealib::RecursiveDyckMatchingStructure<BlockType>::RecursiveDyckMatchingStructure(
    const Sealib::Bitset<BlockType> &word_,
    unsigned int recursions) :
    DyckMatchingStructure<BlockType>(word_),
    segments(static_cast<unsigned int>(word_.size() / segmentLength)),
    lastSegment(static_cast<BlockType>(word_.size() % segmentLength)),
    pioneerRankSelect(initializePioneerRankSelectBitset()),
    pioneerMatchingStructure(nullptr) {
    unsigned long maxRank = pioneerRankSelect.rank(pioneerRankSelect.size());
    if (maxRank > 0) {
        Sealib::Bitset<BlockType> pioneerWord(maxRank);
        for (unsigned long i = 0; i < maxRank; i++) {
            unsigned long idx = pioneerRankSelect.select(i + 1);
            pioneerWord[i] = DyckMatchingStructure<BlockType>::word[idx - 1];
        }
        if (recursions > 0) {
            pioneerMatchingStructure =
                new Sealib::RecursiveDyckMatchingStructure<BlockType>(pioneerWord, recursions - 1);
        } else {
            pioneerMatchingStructure =
                new Sealib::DyckMatchingStructure<BlockType>(pioneerWord);
        }
    }
}
template <typename BlockType>
Sealib::RecursiveDyckMatchingStructure<BlockType>::RecursiveDyckMatchingStructure(
    const Sealib::Bitset<BlockType> &word_)
    : RecursiveDyckMatchingStructure(word_, 2) {
}
template <typename BlockType>
const Sealib::Bitset<BlockType>
    Sealib::RecursiveDyckMatchingStructure<BlockType>::initializePioneerRankSelectBitset() {
    Sealib::Bitset<BlockType>
        pioneerRankSelectBitset(DyckMatchingStructure<BlockType>::word.size());

    for (unsigned int i = 0; i < segments; i++) {
        unsigned long beg = segmentLength * i;
        BlockType segment = DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

        unsigned long leftPioneer = LocalDyckTable<BlockType>::getLocalData(segment).leftPioneer;
        unsigned long rightPioneer = LocalDyckTable<BlockType>::getLocalData(segment).rightPioneer;

        if (leftPioneer != LocalDyckTable<BlockType>::Data::npos) {
            if (!pioneerRankSelectBitset[beg + leftPioneer]) {
                pioneerRankSelectBitset[beg + leftPioneer].flip();

                unsigned long matchedPioneer =
                    DyckMatchingStructure<BlockType>::getMatchNaive(
                        DyckMatchingStructure<BlockType>::word,
                        beg + leftPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
        if (rightPioneer != LocalDyckTable<BlockType>::Data::npos) {
            if (!pioneerRankSelectBitset[beg + rightPioneer]) {
                pioneerRankSelectBitset[beg + rightPioneer].flip();

                unsigned long matchedPioneer =
                    DyckMatchingStructure<BlockType>::getMatchNaive(
                        DyckMatchingStructure<BlockType>::word,
                        beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }
    if (lastSegment != 0) {
        unsigned long beg = segmentLength * segments;
        BlockType segment =
            DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

        for (BlockType c = lastSegment; c < segmentLength; c += 2) {
            segment |= static_cast<BlockType>(1 << c);
        }

        unsigned long rightPioneer =
            LocalDyckTable<BlockType>::getLocalData(segment).rightPioneer;

        if (rightPioneer != LocalDyckTable<BlockType>::Data::npos) {
            if (!pioneerRankSelectBitset[beg + rightPioneer]) {
                pioneerRankSelectBitset[beg + rightPioneer].flip();

                unsigned long matchedPioneer =
                    DyckMatchingStructure<BlockType>::getMatchNaive(
                        DyckMatchingStructure<BlockType>::word,
                        beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }

    return pioneerRankSelectBitset;
}
template <typename BlockType>
unsigned long
Sealib::RecursiveDyckMatchingStructure<BlockType>::getMatch(unsigned long idx) {
    unsigned long segmentId = idx / segmentLength;
    auto bSegmentIdx = static_cast<BlockType>(idx % segmentLength);

    if (segmentId != segments || lastSegment == 0) {
        unsigned long beg = segmentId * segmentLength;
        BlockType segment =
            DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

        unsigned char localMatch =
            LocalDyckTable<BlockType>::getLocalData(segment).localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    } else {
        unsigned long beg = segments * segmentLength;
        BlockType segment =
            DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

        unsigned char localMatch =
            LocalDyckTable<BlockType>::getLocalData(segment).localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    }

    // match is not local
    if (DyckMatchingStructure<BlockType>::word[idx]) {  // opening global
        unsigned long p_wp = pioneerRankSelect.rank(idx + 1);
        unsigned long p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp)) - 1;
        unsigned long pMatch_wp = pioneerMatchingStructure->getMatch(p_wp - 1);
        long pMatch = pioneerRankSelect.select(pMatch_wp + 1) - 1;

        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        unsigned long beg = pSegment * segmentLength;
        BlockType pSeg =
            DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

        int pDepth =
            LocalDyckTable<BlockType>::getLocalData(pSeg).localDepths[pSegmentIdx];
        int bDepth =
            LocalDyckTable<BlockType>::getLocalData(pSeg).localDepths[bSegmentIdx];

        int depthDiff = bDepth - pDepth;

        unsigned long pMatchSegment = static_cast<unsigned long>(pMatch / segmentLength);
        unsigned long matchSegmentIdx = static_cast<unsigned long>(pMatch % segmentLength);

        // not last segment or last segment has normal size,
        // it is certain we can get a subword of length segmentLength
        if (pMatch < segmentLength * segments) {
            beg = pMatchSegment * segmentLength;
            BlockType pMatchSeg =
                DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

            int pMatchDepth =
                LocalDyckTable<BlockType>::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth =
                    LocalDyckTable<BlockType>::getLocalData(pMatchSeg).localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return (pMatchSegment * segmentLength) + i;
                }
            }
        } else {
            beg = segments * segmentLength;
            BlockType pMatchSeg =
                DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

            for (BlockType c = lastSegment; c < segmentLength; c += 2) {
                pMatchSeg |= static_cast<BlockType>(1 << c);
            }

            int pMatchDepth =
                LocalDyckTable<BlockType>::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
            for (long i = matchSegmentIdx; i >= 0; i--) {
                int candidateDepth =
                    LocalDyckTable<BlockType>::getLocalData(pMatchSeg).localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return (pMatchSegment * segmentLength) + i;
                }
            }
        }
    } else {  // closing global
        unsigned long p_wp = pioneerRankSelect.rank(idx + 1);
        unsigned long p =
            pioneerRankSelect.select(static_cast<unsigned int>(p_wp)) - 1;
        if (p < idx) {
            p_wp++;
            p = pioneerRankSelect.select(static_cast<unsigned int>(p_wp)) - 1;
        }
        unsigned long pMatch_wp = pioneerMatchingStructure->getMatch(p_wp - 1);
        unsigned long pMatch = pioneerRankSelect.select(pMatch_wp + 1) - 1;

        unsigned long pSegment = p / segmentLength;
        unsigned long pSegmentIdx = p % segmentLength;

        int pDepth;
        int bDepth;
        int depthDiff;

        if (pMatch < segmentLength * segments) {  // not in last segment
            unsigned long beg = pSegment * segmentLength;
            BlockType pSeg =
                DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

            pDepth =
                LocalDyckTable<BlockType>::getLocalData(pSeg).localDepths[pSegmentIdx];
            bDepth =
                LocalDyckTable<BlockType>::getLocalData(pSeg).localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        } else {
            unsigned long beg = segments * segmentLength;
            BlockType pSeg =
                DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

            for (BlockType c = lastSegment; c < segmentLength; c += 2) {
                pSeg |= static_cast<BlockType>(1 << c);
            }

            pDepth =
                LocalDyckTable<BlockType>::getLocalData(pSeg).localDepths[pSegmentIdx];
            bDepth =
                LocalDyckTable<BlockType>::getLocalData(pSeg).localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        }

        unsigned long pMatchSegment = pMatch / segmentLength;
        unsigned long matchSegmentIdx = pMatch % segmentLength;

        unsigned long beg = pMatchSegment * segmentLength;
        BlockType pMatchSeg =
            DyckMatchingStructure<BlockType>::word.getShiftedBlock(beg);

        int pMatchDepth =
            LocalDyckTable<BlockType>::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
        for (unsigned long i = matchSegmentIdx; i < segmentLength; i++) {
            int candidateDepth =
                LocalDyckTable<BlockType>::getLocalData(pMatchSeg).localDepths[i];
            int candidateDepthDifference = candidateDepth - pMatchDepth;
            if (candidateDepthDifference == depthDiff) {  // i is match
                return (pMatchSegment * segmentLength) + i;
            }
        }
    }
    return idx;
}

namespace Sealib {
template
class RecursiveDyckMatchingStructure<unsigned char>;
template
class RecursiveDyckMatchingStructure<unsigned short>;
}  // namespace Sealib


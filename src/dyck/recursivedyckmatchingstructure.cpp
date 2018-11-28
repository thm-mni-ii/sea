#include <sealib/recursivedyckmatchingstructure.h>
#include <cmath>
#include <iostream>

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(
    const Sealib::Bitset<uint8_t> &word_,
    uint32_t recursions) :
    DyckMatchingStructure(word_),
    segments(static_cast<uint32_t>(word_.size() / segmentLength)),
    lastSegment(static_cast<uint8_t>(word_.size() % segmentLength)),
    pioneerRankSelect(initializePioneerRankSelectBitset()),
    pioneerMatchingStructure(nullptr) {
    uint64_t maxRank = pioneerRankSelect.rank(pioneerRankSelect.size());
    if (maxRank > 0) {
        Sealib::Bitset<uint8_t> pioneerWord(maxRank);
        for (uint64_t i = 0; i < maxRank; i++) {
            uint64_t idx = pioneerRankSelect.select(i + 1);
            pioneerWord[i] = word[idx - 1];
        }
        if (recursions > 0) {
            pioneerMatchingStructure =
                new Sealib::RecursiveDyckMatchingStructure(pioneerWord, recursions - 1);
        } else {
            pioneerMatchingStructure =
                new Sealib::DyckMatchingStructure(pioneerWord);
        }
    }
}

Sealib::RecursiveDyckMatchingStructure::RecursiveDyckMatchingStructure(
    const Sealib::Bitset<uint8_t> &word_)
    : RecursiveDyckMatchingStructure(word_, 2) {
}

const Sealib::Bitset<uint8_t>
    Sealib::RecursiveDyckMatchingStructure::initializePioneerRankSelectBitset() {
    Sealib::Bitset<uint8_t> pioneerRankSelectBitset(word.size());

    for (uint32_t i = 0; i < segments; i++) {
        uint64_t beg = segmentLength * i;
        uint8_t segment = word.getShiftedBlock(beg);

        uint64_t leftPioneer = LocalDyckTable::getLocalData(segment).leftPioneer;
        uint64_t rightPioneer = LocalDyckTable::getLocalData(segment).rightPioneer;

        if (leftPioneer != (uint8_t) -1) {
            if (!pioneerRankSelectBitset[beg + leftPioneer]) {
                pioneerRankSelectBitset[beg + leftPioneer].flip();

                uint64_t matchedPioneer =
                    DyckMatchingStructure::getMatchNaive(word, beg + leftPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
        if (rightPioneer != (uint8_t) -1) {
            if (!pioneerRankSelectBitset[beg + rightPioneer]) {
                pioneerRankSelectBitset[beg + rightPioneer].flip();

                uint64_t matchedPioneer =
                    DyckMatchingStructure::getMatchNaive(word, beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }
    if (lastSegment != 0) {
        uint64_t beg = segmentLength * segments;
        uint8_t segment = word.getShiftedBlock(beg);

        for (uint32_t c = lastSegment; c < segmentLength; c += 2) {
            segment |= static_cast<uint8_t>(1 << c);
        }

        uint64_t rightPioneer = LocalDyckTable::getLocalData(segment).rightPioneer;

        if (rightPioneer != (uint8_t) -1) {
            if (!pioneerRankSelectBitset[beg + rightPioneer]) {
                pioneerRankSelectBitset[beg + rightPioneer].flip();

                uint64_t matchedPioneer =
                    DyckMatchingStructure::getMatchNaive(word, beg + rightPioneer);

                if (!pioneerRankSelectBitset[matchedPioneer]) {
                    pioneerRankSelectBitset[matchedPioneer].flip();
                }
            }
        }
    }

    return pioneerRankSelectBitset;
}

uint64_t Sealib::RecursiveDyckMatchingStructure::getMatch(uint64_t idx) {
    uint64_t segmentId = idx / segmentLength;
    auto bSegmentIdx = static_cast<uint8_t>(idx % segmentLength);

    if (segmentId != segments || lastSegment == 0) {
        uint64_t beg = segmentId * segmentLength;
        uint8_t segment = word.getShiftedBlock(beg);

        uint8_t localMatch =
            LocalDyckTable::getLocalData(segment).localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    } else {
        uint64_t beg = segments * segmentLength;
        uint8_t segment = word.getShiftedBlock(beg);

        uint8_t localMatch =
            LocalDyckTable::getLocalData(segment).localMatches[bSegmentIdx];
        if (localMatch != bSegmentIdx) {
            return beg + localMatch;
        }
    }

    // match is not local
    if (word[idx]) {  // opening global
        uint64_t p_wp = pioneerRankSelect.rank(idx + 1);
        uint64_t p = pioneerRankSelect.select(static_cast<uint32_t>(p_wp)) - 1;
        uint64_t pMatch_wp = pioneerMatchingStructure->getMatch(p_wp - 1);
        uint64_t pMatch = pioneerRankSelect.select(pMatch_wp + 1) - 1;

        uint64_t pSegment = p / segmentLength;
        uint64_t pSegmentIdx = p % segmentLength;

        uint64_t beg = pSegment * segmentLength;
        uint8_t pSeg = word.getShiftedBlock(beg);

        int pDepth = LocalDyckTable::getLocalData(pSeg).localDepths[pSegmentIdx];
        int bDepth = LocalDyckTable::getLocalData(pSeg).localDepths[bSegmentIdx];

        int depthDiff = bDepth - pDepth;

        uint64_t pMatchSegment = static_cast<uint64_t>(pMatch / segmentLength);
        uint64_t matchSegmentIdx = static_cast<uint64_t>(pMatch % segmentLength);

        // not last segment or last segment has normal size,
        // it is certain we can get a subword of length segmentLength
        if (pMatch < segmentLength * segments) {
            beg = pMatchSegment * segmentLength;
            uint8_t pMatchSeg = word.getShiftedBlock(beg);

            int pMatchDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
            for (int64_t i = static_cast<int64_t >(matchSegmentIdx); i >= 0; i--) {
                int candidateDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return (pMatchSegment * segmentLength) + static_cast<uint32_t >(i);
                }
            }
        } else {
            beg = segments * segmentLength;
            uint8_t pMatchSeg = word.getShiftedBlock(beg);

            for (uint32_t c = lastSegment; c < segmentLength; c += 2) {
                pMatchSeg |= static_cast<uint8_t>(1 << c);
            }

            int pMatchDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
            for (int64_t i = static_cast<int64_t >(matchSegmentIdx); i >= 0; i--) {
                int candidateDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[i];
                int candidateDepthDifference = pMatchDepth - candidateDepth;
                if (candidateDepthDifference == depthDiff) {  // i is match
                    return (pMatchSegment * segmentLength) + static_cast<uint32_t >(i);
                }
            }
        }
    } else {  // closing global
        uint64_t p_wp = pioneerRankSelect.rank(idx + 1);
        uint64_t p = pioneerRankSelect.select(static_cast<uint32_t>(p_wp)) - 1;
        if (p < idx) {
            p_wp++;
            p = pioneerRankSelect.select(static_cast<uint32_t>(p_wp)) - 1;
        }
        uint64_t pMatch_wp = pioneerMatchingStructure->getMatch(p_wp - 1);
        uint64_t pMatch = pioneerRankSelect.select(pMatch_wp + 1) - 1;

        uint64_t pSegment = p / segmentLength;
        uint64_t pSegmentIdx = p % segmentLength;

        int pDepth;
        int bDepth;
        int depthDiff;

        if (pMatch < segmentLength * segments) {  // not in last segment
            uint64_t beg = pSegment * segmentLength;
            uint8_t pSeg = word.getShiftedBlock(beg);

            pDepth = LocalDyckTable::getLocalData(pSeg).localDepths[pSegmentIdx];
            bDepth = LocalDyckTable::getLocalData(pSeg).localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        } else {
            uint64_t beg = segments * segmentLength;
            uint8_t pSeg = word.getShiftedBlock(beg);

            for (uint32_t c = lastSegment; c < segmentLength; c += 2) {
                pSeg |= static_cast<uint8_t>(1 << c);
            }

            pDepth = LocalDyckTable::getLocalData(pSeg).localDepths[pSegmentIdx];
            bDepth = LocalDyckTable::getLocalData(pSeg).localDepths[bSegmentIdx];

            depthDiff = pDepth - bDepth;
        }

        uint64_t pMatchSegment = pMatch / segmentLength;
        uint64_t matchSegmentIdx = pMatch % segmentLength;

        uint64_t beg = pMatchSegment * segmentLength;
        uint8_t pMatchSeg = word.getShiftedBlock(beg);

        int pMatchDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[matchSegmentIdx];
        for (uint64_t i = matchSegmentIdx; i < segmentLength; i++) {
            int candidateDepth = LocalDyckTable::getLocalData(pMatchSeg).localDepths[i];
            int candidateDepthDifference = candidateDepth - pMatchDepth;
            if (candidateDepthDifference == depthDiff) {  // i is match
                return (pMatchSegment * segmentLength) + i;
            }
        }
    }
    return idx;
}


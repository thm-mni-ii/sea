#include "localranktable.h"
#include "sharedrankstructure.h"
#include <iostream>
#include <cmath>
#include <utility>

uint64_t Sealib::SharedRankStructure::rank(uint64_t k) const {
    if (k == 0 || k > maxRank) {
        return INVALID;
    }
    uint64_t segmentIdx = (k - 1) / segmentLength;
    uint8_t segment = bitset->getBlock(segmentIdx);
    auto localIdx = static_cast<uint8_t>((k - 1) % segmentLength);
    return setBefore(segmentIdx) + LocalRankTable::getLocalRank(segment, localIdx);
}

Sealib::SharedRankStructure::SharedRankStructure(
    std::shared_ptr<const Sealib::Bitset<uint8_t>> bitset_) :
    bitset(std::move(bitset_)),
    segmentCount(static_cast<uint32_t>(bitset->size() / segmentLength)) {
    auto lastSeg = static_cast<uint8_t>((bitset->size() % segmentLength));

    if ((lastSeg != 0) && bitset->size() != 0) {
        segmentCount++;
    }

    maxRank =
        segmentCount == 0 ?
        0 : segmentLength * (lastSeg == 0 ? segmentCount : segmentCount - 1) + lastSeg;
    nonEmptySegments.reserve(segmentCount);

    for (uint32_t i = 0; i < segmentCount; i++) {
        uint8_t segment = bitset->getBlock(i);
        if (LocalRankTable::getLocalRank(segment, 7) != 0) {
            nonEmptySegments.push_back(i);
        }
    }

    if (segmentCount != 0) {
        setCountTable.reserve(segmentCount);
        uint32_t cnt = 0;
        for (uint64_t i = 0; i < segmentCount - 1; i++) {
            uint8_t segment = bitset->getBlock(i);
            cnt += LocalRankTable::getLocalRank(segment, 7);
            setCountTable.push_back(cnt);
        }
    }
}

Sealib::SharedRankStructure::SharedRankStructure() : bitset(0) {
}

uint32_t Sealib::SharedRankStructure::getSegmentCount() const {
    return segmentCount;
}

uint32_t Sealib::SharedRankStructure::setBefore(uint64_t segment) const {
    if (segment == 0) return 0;
    return setCountTable[segment - 1];
}

uint8_t Sealib::SharedRankStructure::getSegmentLength() const {
    return segmentLength;
}
uint64_t Sealib::SharedRankStructure::size() const {
    return bitset->size();
}
const Sealib::Bitset<uint8_t> &Sealib::SharedRankStructure::getBitset() const {
    return (*bitset.get());
}
uint32_t Sealib::SharedRankStructure::getMaxRank() const {
    return maxRank;
}
const std::vector<uint32_t> &Sealib::SharedRankStructure::getSetCountTable() const {
    return setCountTable;
}
const std::vector<uint32_t> &Sealib::SharedRankStructure::getNonEmptySegments() const {
    return nonEmptySegments;
}

Sealib::SharedRankStructure::~SharedRankStructure() = default;

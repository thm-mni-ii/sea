#include "./simpletrail.h"

void Sealib::SimpleTrail::insertSubTrail(const SimpleTrail &subTrail, uint64_t idx) {
    std::vector<std::tuple<uint32_t, uint32_t>> oldTrail = trail;
    trail.resize(trail.size() + subTrail.getTrail().size());
    for (uint64_t i = 0; i < subTrail.getTrail().size(); i++) {
        trail[i + idx] = subTrail.getTrail()[i];
    }
    for (uint64_t i = idx + subTrail.getTrail().size(); i < trail.size(); i++) {
        std::tuple<uint32_t, uint32_t> oldVal = oldTrail[i - subTrail.getTrail().size()];
        trail[i] = oldVal;
    }
}

void Sealib::SimpleTrail::addArc(std::tuple<uint32_t, uint32_t> arc) {
    trail.push_back(arc);
}

const std::vector<std::tuple<uint32_t, uint32_t>> &Sealib::SimpleTrail::getTrail() const {
    return trail;
}

Sealib::SimpleTrail::SimpleTrail() = default;

std::tuple<uint32_t, uint32_t> Sealib::SimpleTrail::getOutgoingFrom(uint32_t u) const {
    for (uint32_t i = 0; i < trail.size(); i++) {
        if (std::get<0>(trail[i]) == u) {
            return trail[i];
        }
    }
    return std::make_tuple((uint32_t) -1, (uint32_t) -1);
}

uint64_t
Sealib::SimpleTrail::getFirstIndexOf(std::tuple<uint32_t, uint32_t> arc) const {
    for (uint32_t i = 0; i < trail.size(); i++) {
        if (trail[i] == arc) {
            return i;
        }
    }
    return (uint64_t) -1;
}

void Sealib::SimpleTrail::pushBackSubTrail(const Sealib::SimpleTrail &subTrail) {
    uint64_t oldSize = trail.size();
    trail.resize(trail.size() + subTrail.getTrail().size());
    for (uint64_t i = oldSize; i < trail.size(); i++) {
        trail[i] = subTrail.getTrail()[i - oldSize];
    }
}
